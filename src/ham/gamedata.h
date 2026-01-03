#ifndef HAM_GAMEDATA_H
#define HAM_GAMEDATA_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cctype>

namespace Ham {

struct FunctionOffset {
    int windows = -1;
    int linux_offset = -1;
    int mac = -1;

    int getCurrent() const {
#if defined(_WIN32)
        return windows;
#elif defined(__APPLE__)
        return mac;
#else
        return linux_offset;
#endif
    }

    bool isValid() const {
        return getCurrent() >= 0;
    }
};

class GameData {
public:
    bool loadFromFile(const std::string& filepath);
    bool loadFromString(const std::string& content);

    FunctionOffset getOffset(const std::string& name) const;
    bool hasOffset(const std::string& name) const;

    int getPevOffset() const { return m_pevOffset; }
    int getBaseOffset() const { return m_baseOffset; }

private:
    std::map<std::string, FunctionOffset> m_offsets;
    int m_pevOffset = 4;
    int m_baseOffset = 0;

    enum class TokenType {
        String,
        OpenBrace,
        CloseBrace,
        EndOfFile,
        Error
    };

    struct Token {
        TokenType type;
        std::string value;
    };

    class Tokenizer {
    public:
        Tokenizer(const std::string& input) : m_input(input), m_pos(0) {}

        Token next() {
            skipWhitespaceAndComments();

            if (m_pos >= m_input.length()) {
                return {TokenType::EndOfFile, ""};
            }

            char c = m_input[m_pos];

            if (c == '{') {
                m_pos++;
                return {TokenType::OpenBrace, "{"};
            }
            if (c == '}') {
                m_pos++;
                return {TokenType::CloseBrace, "}"};
            }
            if (c == '"') {
                return readString();
            }

            // Unquoted string (identifier)
            return readIdentifier();
        }

    private:
        const std::string& m_input;
        size_t m_pos;

        void skipWhitespaceAndComments() {
            while (m_pos < m_input.length()) {
                char c = m_input[m_pos];

                if (std::isspace(c)) {
                    m_pos++;
                    continue;
                }

                // C-style comment
                if (c == '/' && m_pos + 1 < m_input.length()) {
                    if (m_input[m_pos + 1] == '/') {
                        // Line comment
                        while (m_pos < m_input.length() && m_input[m_pos] != '\n') {
                            m_pos++;
                        }
                        continue;
                    }
                    if (m_input[m_pos + 1] == '*') {
                        // Block comment
                        m_pos += 2;
                        while (m_pos + 1 < m_input.length()) {
                            if (m_input[m_pos] == '*' && m_input[m_pos + 1] == '/') {
                                m_pos += 2;
                                break;
                            }
                            m_pos++;
                        }
                        continue;
                    }
                }

                break;
            }
        }

        Token readString() {
            m_pos++;  // Skip opening quote
            std::string value;
            while (m_pos < m_input.length() && m_input[m_pos] != '"') {
                if (m_input[m_pos] == '\\' && m_pos + 1 < m_input.length()) {
                    m_pos++;
                }
                value += m_input[m_pos++];
            }
            if (m_pos < m_input.length()) {
                m_pos++;  // Skip closing quote
            }
            return {TokenType::String, value};
        }

        Token readIdentifier() {
            std::string value;
            while (m_pos < m_input.length()) {
                char c = m_input[m_pos];
                if (std::isspace(c) || c == '{' || c == '}' || c == '"') {
                    break;
                }
                value += c;
                m_pos++;
            }
            return {TokenType::String, value};
        }
    };

    bool parseGames(Tokenizer& tok);
    bool parseGameSection(Tokenizer& tok);
    bool parseOffsets(Tokenizer& tok);
    bool parseOffsetEntry(Tokenizer& tok, const std::string& name);

    int parseNumber(const std::string& str) {
        if (str.empty()) return -1;
        try {
            if (str.length() > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
                return std::stoi(str.substr(2), nullptr, 16);
            }
            return std::stoi(str);
        } catch (...) {
            return -1;
        }
    }
};

} // namespace Ham

#endif // HAM_GAMEDATA_H

#include "gamedata.h"
#include <algorithm>

namespace Ham {

bool GameData::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return loadFromString(buffer.str());
}

bool GameData::loadFromString(const std::string& content) {
    Tokenizer tok(content);

    Token t = tok.next();
    if (t.type != TokenType::String) {
        return false;
    }

    // Expect "Games" as root
    std::string rootName = t.value;
    std::transform(rootName.begin(), rootName.end(), rootName.begin(), ::tolower);

    if (rootName == "games") {
        return parseGames(tok);
    }

    return false;
}

bool GameData::parseGames(Tokenizer& tok) {
    Token t = tok.next();
    if (t.type != TokenType::OpenBrace) {
        return false;
    }

    while (true) {
        t = tok.next();
        if (t.type == TokenType::CloseBrace) {
            return true;
        }
        if (t.type != TokenType::String) {
            return false;
        }

        // Game section name (e.g., "#default", "valve", etc.)
        std::string sectionName = t.value;

        if (!parseGameSection(tok)) {
            return false;
        }
    }
}

bool GameData::parseGameSection(Tokenizer& tok) {
    Token t = tok.next();
    if (t.type != TokenType::OpenBrace) {
        return false;
    }

    while (true) {
        t = tok.next();
        if (t.type == TokenType::CloseBrace) {
            return true;
        }
        if (t.type != TokenType::String) {
            return false;
        }

        std::string sectionName = t.value;
        std::transform(sectionName.begin(), sectionName.end(), sectionName.begin(), ::tolower);

        if (sectionName == "offsets") {
            if (!parseOffsets(tok)) {
                return false;
            }
        } else {
            // Skip unknown sections
            t = tok.next();
            if (t.type == TokenType::OpenBrace) {
                int depth = 1;
                while (depth > 0) {
                    t = tok.next();
                    if (t.type == TokenType::OpenBrace) depth++;
                    else if (t.type == TokenType::CloseBrace) depth--;
                    else if (t.type == TokenType::EndOfFile) return false;
                }
            }
        }
    }
}

bool GameData::parseOffsets(Tokenizer& tok) {
    Token t = tok.next();
    if (t.type != TokenType::OpenBrace) {
        return false;
    }

    while (true) {
        t = tok.next();
        if (t.type == TokenType::CloseBrace) {
            return true;
        }
        if (t.type != TokenType::String) {
            return false;
        }

        std::string offsetName = t.value;

        if (!parseOffsetEntry(tok, offsetName)) {
            return false;
        }
    }
}

bool GameData::parseOffsetEntry(Tokenizer& tok, const std::string& name) {
    Token t = tok.next();
    if (t.type != TokenType::OpenBrace) {
        return false;
    }

    FunctionOffset offset;

    while (true) {
        t = tok.next();
        if (t.type == TokenType::CloseBrace) {
            break;
        }
        if (t.type != TokenType::String) {
            return false;
        }

        std::string platform = t.value;
        std::transform(platform.begin(), platform.end(), platform.begin(), ::tolower);

        t = tok.next();
        if (t.type != TokenType::String) {
            return false;
        }

        int value = parseNumber(t.value);

        if (platform == "windows") {
            offset.windows = value;
        } else if (platform == "linux") {
            offset.linux_offset = value;
        } else if (platform == "mac") {
            offset.mac = value;
        }
    }

    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    // Store special offsets
    if (lowerName == "pev") {
        m_pevOffset = offset.getCurrent();
    } else if (lowerName == "base") {
        m_baseOffset = offset.getCurrent();
    }

    m_offsets[lowerName] = offset;
    return true;
}

FunctionOffset GameData::getOffset(const std::string& name) const {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    auto it = m_offsets.find(lowerName);
    if (it != m_offsets.end()) {
        return it->second;
    }
    return FunctionOffset{};
}

bool GameData::hasOffset(const std::string& name) const {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    return m_offsets.find(lowerName) != m_offsets.end();
}

} // namespace Ham

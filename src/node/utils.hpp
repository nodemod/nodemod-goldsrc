#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include "node.h"
#include "v8.h"
#include "extdll.h"

namespace utils {

inline v8::Local<v8::Array> vect2js(v8::Isolate *isolate, const vec3_t &_array) {
		v8::Locker locker(isolate);
  v8::Local<v8::Array> array = v8::Array::New(isolate, 3);

  array->Set(isolate->GetCurrentContext(), 0, v8::Number::New(isolate, _array.x)).Check();
  array->Set(isolate->GetCurrentContext(), 1, v8::Number::New(isolate, _array.y)).Check();
  array->Set(isolate->GetCurrentContext(), 2, v8::Number::New(isolate, _array.z)).Check();

  return array;
}

inline void js2vect(v8::Isolate *isolate, v8::Local<v8::Array> array, vec3_t &vector) {
		v8::Locker locker(isolate);
  vector.x = array->Get(isolate->GetCurrentContext(), 0).ToLocalChecked()->ToNumber(isolate->GetCurrentContext()).ToLocalChecked()->Value();
  vector.y = array->Get(isolate->GetCurrentContext(), 1).ToLocalChecked()->ToNumber(isolate->GetCurrentContext()).ToLocalChecked()->Value();
  vector.z = array->Get(isolate->GetCurrentContext(), 2).ToLocalChecked()->ToNumber(isolate->GetCurrentContext()).ToLocalChecked()->Value();
}

	inline const char* check_string_conversion(const v8::String::Utf8Value& value)
	{
		return *value ? *value : "<string conversion failed>";
	}

	inline const char* js_to_cstr(v8::Isolate *isolate, const v8::Local<v8::Value>& val)
	{
		const v8::String::Utf8Value jsString(isolate, val);
		const char* str = check_string_conversion(jsString);
		return str;
	}

	inline std::string js_to_string(v8::Isolate *isolate, const v8::Local<v8::Value>& val)
	{
		const char* str;
		const v8::String::Utf8Value jsString(isolate, val);
		str = check_string_conversion(jsString);
		return std::string(str);
	}

	// TODO fix terrible memory leaking
	inline const char* js2string(v8:: Isolate * isolate, const v8::Local<v8::Value>& value) {
	v8::String::Utf8Value _str(isolate, value);
					const char* str(*_str);
					size_t slen = strlen(str);
					char* mystr = new char[slen + 1];
					for (size_t x = 0; x < slen; x++)
					{
						mystr[x] = str[x];
					}
					mystr[slen] = '\0';

	return mystr;
}

	inline std::vector<std::string> split(const std::string& s, char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	inline void* jsToBytes(v8::Isolate *isolate, const v8::Local<v8::Value>& val) {
		if (val.IsEmpty() || !val->IsExternal()) {
			return nullptr;
		}

		v8::Local<v8::External> ext = val.As<v8::External>();
		return ext->Value();
	}

	// Convert JS value to generic pointer (for pointer parameter handling)
	inline void* jsToPointer(v8::Isolate *isolate, const v8::Local<v8::Value>& val) {
		if (val.IsEmpty() || !val->IsExternal()) {
			return nullptr;
		}

		v8::Local<v8::External> ext = val.As<v8::External>();
		return ext->Value();
	}

	// Convert float array to JS array
	inline v8::Local<v8::Array> floatArrayToJS(v8::Isolate *isolate, const float* array, size_t length) {
		v8::Locker locker(isolate);
		v8::Local<v8::Array> jsArray = v8::Array::New(isolate, length);

		for (size_t i = 0; i < length; i++) {
			jsArray->Set(isolate->GetCurrentContext(), i, v8::Number::New(isolate, array[i])).Check();
		}

		return jsArray;
	}

	// Overload for single float pointer - explicit single element
	inline v8::Local<v8::Array> floatArrayToJS(v8::Isolate *isolate, const float* array) {
		if (!array) {
			return v8::Array::New(isolate, 0);
		}
		// For single float pointer, return single element array
		return floatArrayToJS(isolate, array, 1);
	}

	// Convert int array to JS array
	inline v8::Local<v8::Array> intArrayToJS(v8::Isolate *isolate, const int* array, size_t length) {
		v8::Locker locker(isolate);
		v8::Local<v8::Array> jsArray = v8::Array::New(isolate, length);

		for (size_t i = 0; i < length; i++) {
			jsArray->Set(isolate->GetCurrentContext(), i, v8::Number::New(isolate, array[i])).Check();
		}

		return jsArray;
	}

	// Overload for single int pointer
	inline v8::Local<v8::Array> intArrayToJS(v8::Isolate *isolate, const int* array) {
		if (!array) {
			return v8::Array::New(isolate, 0);
		}
		return intArrayToJS(isolate, array, 1);
	}

	// Convert char** (string array) to JS array
	inline v8::Local<v8::Array> stringArrayToJS(v8::Isolate *isolate, char** strings, size_t length) {
		v8::Locker locker(isolate);
		v8::Local<v8::Array> jsArray = v8::Array::New(isolate, length);

		for (size_t i = 0; i < length; i++) {
			if (strings[i]) {
				jsArray->Set(isolate->GetCurrentContext(), i, 
					v8::String::NewFromUtf8(isolate, strings[i]).ToLocalChecked()).Check();
			} else {
				jsArray->Set(isolate->GetCurrentContext(), i, v8::Null(isolate)).Check();
			}
		}

		return jsArray;
	}

	// Overload for null-terminated string array
	inline v8::Local<v8::Array> stringArrayToJS(v8::Isolate *isolate, char** strings) {
		if (!strings) {
			return v8::Array::New(isolate, 0);
		}

		// Count strings until null pointer
		size_t length = 0;
		while (strings[length] != nullptr) {
			length++;
		}

		return stringArrayToJS(isolate, strings, length);
	}

	// Convert unsigned char array to JS array (byte array)
	inline v8::Local<v8::Array> byteArrayToJS(v8::Isolate *isolate, const unsigned char* bytes, size_t length) {
		v8::Locker locker(isolate);
		v8::Local<v8::Array> jsArray = v8::Array::New(isolate, length);

		for (size_t i = 0; i < length; i++) {
			jsArray->Set(isolate->GetCurrentContext(), i, v8::Number::New(isolate, bytes[i])).Check();
		}

		return jsArray;
	}

	// Overload for single byte pointer (assumes length 1)
	inline v8::Local<v8::Array> byteArrayToJS(v8::Isolate *isolate, const unsigned char* bytes) {
		if (!bytes) {
			return v8::Array::New(isolate, 0);
		}
		return byteArrayToJS(isolate, bytes, 1);
	}
}
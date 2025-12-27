#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <cmath>
#include <cstring>
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
	inline const char* js2string(v8::Isolate* isolate, const v8::Local<v8::Value>& value) {
		if (value.IsEmpty() || value->IsNullOrUndefined()) {
			char* empty = new char[1];
			empty[0] = '\0';
			return empty;
		}
		v8::String::Utf8Value _str(isolate, value);
		const char* str(*_str);
		if (!str) {
			char* empty = new char[1];
			empty[0] = '\0';
			return empty;
		}
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
		if (val.IsEmpty() || val->IsNull() || val->IsUndefined()) {
			return nullptr;
		}

		// Handle External objects (existing wrapped pointers)
		if (val->IsExternal()) {
			v8::Local<v8::External> ext = val.As<v8::External>();
			return ext->Value();
		}

		// Handle ArrayBuffer objects (for file operations and binary data)
		if (val->IsArrayBuffer()) {
			v8::Local<v8::ArrayBuffer> buffer = val.As<v8::ArrayBuffer>();
			std::shared_ptr<v8::BackingStore> backing = buffer->GetBackingStore();
			return backing->Data();
		}

		// Handle SharedArrayBuffer objects
		if (val->IsSharedArrayBuffer()) {
			v8::Local<v8::SharedArrayBuffer> buffer = val.As<v8::SharedArrayBuffer>();
			std::shared_ptr<v8::BackingStore> backing = buffer->GetBackingStore();
			return backing->Data();
		}

		// Handle all TypedArray types
		if (val->IsTypedArray()) {
			v8::Local<v8::TypedArray> typedArray = val.As<v8::TypedArray>();
			v8::Local<v8::ArrayBuffer> buffer = typedArray->Buffer();
			std::shared_ptr<v8::BackingStore> backing = buffer->GetBackingStore();
			return static_cast<uint8_t*>(backing->Data()) + typedArray->ByteOffset();
		}

		// Handle DataView objects
		if (val->IsDataView()) {
			v8::Local<v8::DataView> dataView = val.As<v8::DataView>();
			v8::Local<v8::ArrayBuffer> buffer = dataView->Buffer();
			std::shared_ptr<v8::BackingStore> backing = buffer->GetBackingStore();
			return static_cast<uint8_t*>(backing->Data()) + dataView->ByteOffset();
		}

		// Handle Node.js Buffer objects (which are Uint8Array under the hood)
		// Buffer check is already covered by IsTypedArray above

		// Handle String objects - convert to C string
		if (val->IsString()) {
			// Use thread-local storage for string conversion
			static thread_local char stringBuffers[2][64]; // 2 buffers of 64 chars each
			static thread_local int stringIndex = 0;
			
			char* currentString = stringBuffers[stringIndex];
			stringIndex = (stringIndex + 1) % 2;
			
			v8::String::Utf8Value utf8(isolate, val);
			const char* str = *utf8 ? *utf8 : "";
			strncpy(currentString, str, 63);
			currentString[63] = '\0';
			return (void*)currentString;
		}

		// Handle Objects with internal pointers (wrapped structures)
		if (val->IsObject()) {
			v8::Local<v8::Object> obj = val.As<v8::Object>();
			v8::Local<v8::Context> context = isolate->GetCurrentContext();
			
			// Check for "ptr" or "pointer" property
			v8::Local<v8::String> ptrKey = v8::String::NewFromUtf8(isolate, "ptr").ToLocalChecked();
			v8::Local<v8::String> pointerKey = v8::String::NewFromUtf8(isolate, "pointer").ToLocalChecked();
			
			if (obj->Has(context, ptrKey).ToChecked()) {
				v8::Local<v8::Value> ptrVal = obj->Get(context, ptrKey).ToLocalChecked();
				if (ptrVal->IsNumber()) {
					// Treat number as raw pointer address
					intptr_t address = static_cast<intptr_t>(ptrVal->NumberValue(context).ToChecked());
					return reinterpret_cast<void*>(address);
				}
			} else if (obj->Has(context, pointerKey).ToChecked()) {
				v8::Local<v8::Value> ptrVal = obj->Get(context, pointerKey).ToLocalChecked();
				if (ptrVal->IsNumber()) {
					intptr_t address = static_cast<intptr_t>(ptrVal->NumberValue(context).ToChecked());
					return reinterpret_cast<void*>(address);
				}
			}
			
			// Check for internal fields (wrapped C++ objects)
			if (obj->InternalFieldCount() > 0) {
				void* ptr = obj->GetAlignedPointerFromInternalField(0);
				if (ptr) return ptr;
			}
		}

		// Handle Arrays - convert to float array (most common case for vectors)
		if (val->IsArray()) {
			v8::Local<v8::Array> arr = val.As<v8::Array>();
			uint32_t length = arr->Length();
			
			// Circular buffer system to support multiple arrays in same call
			static thread_local float floatBuffers[8][16]; // 8 buffers of 16 elements each
			static thread_local int bufferIndex = 0;
			
			// Get next buffer in rotation
			float* currentBuffer = floatBuffers[bufferIndex];
			bufferIndex = (bufferIndex + 1) % 8;
			
			if (length > 16) length = 16;
			
			v8::Local<v8::Context> context = isolate->GetCurrentContext();
			for (uint32_t i = 0; i < length; i++) {
				v8::Local<v8::Value> element = arr->Get(context, i).ToLocalChecked();
				currentBuffer[i] = element->NumberValue(context).ToChecked();
			}
			return (void*)currentBuffer;
		}


		// Handle BigInt as raw pointer address
		if (val->IsBigInt()) {
			v8::Local<v8::BigInt> bigint = val.As<v8::BigInt>();
			int64_t address = bigint->Int64Value();
			return reinterpret_cast<void*>(static_cast<intptr_t>(address));
		}

		// Handle single numbers - could be pointer address or single-element float array
		if (val->IsNumber()) {
			v8::Local<v8::Context> context = isolate->GetCurrentContext();
			double numVal = val->NumberValue(context).ToChecked();
			
			// If it looks like a pointer address (large integer), treat as pointer
			if (numVal > 0x10000 && numVal == floor(numVal)) {
				intptr_t address = static_cast<intptr_t>(numVal);
				return reinterpret_cast<void*>(address);
			}
			
			// Otherwise treat as single-element float array
			static thread_local float singleFloats[8];
			static thread_local int singleIndex = 0;
			
			float* currentSingle = &singleFloats[singleIndex];
			singleIndex = (singleIndex + 1) % 8;
			
			*currentSingle = static_cast<float>(numVal);
			return (void*)currentSingle;
		}

		// Fall back to nullptr for other types
		return nullptr;
	}

	// Convert float array to JS array
	inline v8::Local<v8::Array> floatArrayToJS(v8::Isolate *isolate, const float* array, size_t length) {
		v8::Locker locker(isolate);
		
		if (!array) {
			return v8::Array::New(isolate, 0);
		}
		
		v8::Local<v8::Array> jsArray = v8::Array::New(isolate, length);

		for (size_t i = 0; i < length; i++) {
			jsArray->Set(isolate->GetCurrentContext(), i, v8::Number::New(isolate, array[i])).Check();
		}

		return jsArray;
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
		
		if (!bytes) {
			return v8::Array::New(isolate, 0);
		}
		
		v8::Local<v8::Array> jsArray = v8::Array::New(isolate, length);

		for (size_t i = 0; i < length; i++) {
			jsArray->Set(isolate->GetCurrentContext(), i, v8::Number::New(isolate, bytes[i])).Check();
		}

		return jsArray;
	}
}
#pragma once
#include "v8.h"
#include "extdll.h"

extern enginefuncs_t g_engfuncs;

namespace convert
{
  inline v8::Local<v8::String> str2js(v8::Isolate *isolate, const char *str)
  {
    if (!str) {
      return v8::String::NewFromUtf8(isolate, "", v8::NewStringType::kNormal).ToLocalChecked();
    }
    return v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal).ToLocalChecked();
  };

  inline v8::Local<v8::String> strid2js(v8::Isolate *isolate, int id)
  {
    return convert::str2js(isolate, (*g_engfuncs.pfnSzFromIndex)(id));
  };

  inline const char *js2str(v8::Isolate *isolate, const v8::Local<v8::Value> &value)
  {
    v8::String::Utf8Value _str(isolate, value);
    const char *str(*_str);
    size_t slen = strlen(str);
    char *mystr = new char[slen + 1];
    for (size_t x = 0; x < slen; x++)
    {
      mystr[x] = str[x];
    }
    mystr[slen] = '\0';

    return mystr;

    // regactored chatgpt version
    // v8::String::Utf8Value utf8(isolate, value);
    // std::string str(*utf8);
    // return str.c_str();
  }

  inline int js2int(v8::Isolate *isolate, const v8::Local<v8::Value> &value) {
    return value->Int32Value(isolate->GetCurrentContext()).ToChecked();
  }

  inline void js2fvec3(v8::Isolate *isolate, const v8::Local<v8::Array> &array, float vector[]) {
	  v8::Locker locker(isolate);
	  vector[0] = array->Get(isolate->GetCurrentContext(), 0).ToLocalChecked()->ToNumber(isolate->GetCurrentContext()).ToLocalChecked()->Value();
	  vector[1] = array->Get(isolate->GetCurrentContext(), 1).ToLocalChecked()->ToNumber(isolate->GetCurrentContext()).ToLocalChecked()->Value();
	  vector[2] = array->Get(isolate->GetCurrentContext(), 2).ToLocalChecked()->ToNumber(isolate->GetCurrentContext()).ToLocalChecked()->Value();
  }
}

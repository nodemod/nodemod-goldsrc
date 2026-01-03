#ifndef HAM_BINDINGS_H
#define HAM_BINDINGS_H

#include <v8.h>

namespace Ham {

v8::Local<v8::ObjectTemplate> createHamBindings(v8::Isolate* isolate);

} // namespace Ham

#endif // HAM_BINDINGS_H

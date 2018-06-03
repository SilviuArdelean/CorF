// search.cpp
#include <node.h>
#include <string>

using namespace v8;


void initSearchSearch(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<String>  retval = String::NewFromUtf8(isolate, "world");
  args.GetReturnValue().Set(retval);
}

void effectiveSearch(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  
  v8::String::Utf8Value search_string(args[0]->ToString());

  std::string from = std::string(*search_string);

  auto response = std::string("#search_engine_addon received to search for: ") + from;

  Local<String>  retval = String::NewFromUtf8(isolate, response.c_str());
  args.GetReturnValue().Set(retval);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "initSearchSearch", initSearchSearch);
  NODE_SET_METHOD(exports, "effectiveSearch", effectiveSearch);
}

NODE_MODULE(search_addon, init)


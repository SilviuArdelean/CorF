// search.cpp
#include <node.h>
#include <string>
#include <iostream>
#include "..\include\CacheManager.h"
#include "..\include\Person.h"

using namespace v8;

#define CACHE_SIZE 100
#define DATA_FILE                   "data\\persons_new.json"
#define DATA_FILE_OUTPUT_TESTONLY   "data\\persons_new_test.json"

CacheManager<Person> manager(DATA_FILE, CACHE_SIZE);

void initEngine(const FunctionCallbackInfo<Value>& args) 
{
   if (!manager.Initialize())
   {
      std::cout << "Failed to initialize the cache memory" << std::endl;
   }

   manager.SetSaveAsFileName(DATA_FILE_OUTPUT_TESTONLY);
}

void addItem(const FunctionCallbackInfo<Value>& args)
{
   v8::String::Utf8Value personID(args[0]->ToString()); 
   v8::String::Utf8Value name(args[1]->ToString());
   v8::String::Utf8Value surname(args[2]->ToString());
   v8::String::Utf8Value email(args[3]->ToString());

   std::string    strPersonID = std::string(*personID);
   std::string    strName = std::string(*name);
   std::string    strSurname = std::string(*surname);
   std::string    strEmail = std::string(*email);

   auto    strLastUpdate = string_utils::actual_time2string();

   std::cout << "Add new item: " << strPersonID << "   " << strName << "   " << strSurname << "   " << strEmail << std::endl;

   Person pers(strPersonID, strName, strSurname, strEmail, strLastUpdate);
   manager.AddItem(pers);
   manager.Save(DATA_FILE_OUTPUT_TESTONLY);
}

void deleteItem(const FunctionCallbackInfo<Value>& args)
{
   v8::String::Utf8Value personID(args[0]->ToString());
   std::string    strPersonID = std::string(*personID);
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
  NODE_SET_METHOD(exports, "initEngine", initEngine);
  NODE_SET_METHOD(exports, "addItem", addItem);
  NODE_SET_METHOD(exports, "deleteItem", deleteItem);
  NODE_SET_METHOD(exports, "effectiveSearch",  effectiveSearch);
}

NODE_MODULE(engine_addon, init)


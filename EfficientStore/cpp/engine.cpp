// search.cpp
#include <node.h>
#include <iostream>
#include <string>
#include "..\include\CacheManager.h"
#include "..\include\Person.h"

using namespace v8;

#define CACHE_SIZE 100
#define DATA_FILE "data\\persons.json"
#define DATA_FILE_OUTPUT_TESTONLY "data\\persons_saveas.json"

CacheManager<Person> manager(DATA_FILE, CACHE_SIZE);

void initEngine(const FunctionCallbackInfo<Value>& args) {
  if (!manager.Initialize()) {
    std::cout << "Failed to initialize the cache memory" << std::endl;
  }

  manager.SetSaveAsFileName(DATA_FILE);
}

void addItem(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();
 // v8::String::Utf8Value str(isolate, value->ToString(context).ToLocalChecked());

  if (args.Length() < 4) {
    isolate->ThrowException(String::NewFromUtf8(isolate,
                                                "Wrong number of arguments",
                                                NewStringType::kNormal)
                                .ToLocalChecked());
    return;
  }

  for (int i = 0; i < 4; i++) {
    if (!args[i]->IsString()) {
      isolate->ThrowException(String::NewFromUtf8(isolate, "Wrong arguments",
                                                  NewStringType::kNormal)
                                  .ToLocalChecked());
      return;
    }
  }

  v8::String::Utf8Value personID(isolate,
                                 args[0]->ToString(context).ToLocalChecked());
  v8::String::Utf8Value name(isolate,
                             args[1]->ToString(context).ToLocalChecked());
  v8::String::Utf8Value surname(isolate,
                                args[2]->ToString(context).ToLocalChecked());
  v8::String::Utf8Value email(isolate,
                              args[3]->ToString(context).ToLocalChecked());

  std::string strPersonID = std::string(*personID);
  std::string strName = std::string(*name);
  std::string strSurname = std::string(*surname);
  std::string strEmail = std::string(*email);

  std::string str_ui_response;
  if (strPersonID.empty()) {
    str_ui_response = "<br> No Person ID added. Please add a person ID.";
    MaybeLocal<String> maybeStr = String::NewFromUtf8(
        isolate, str_ui_response.c_str(), NewStringType::kNormal);

    if (maybeStr.IsEmpty()) {
      isolate->ThrowException(String::NewFromUtf8(isolate,
                                                  "String creation failed",
                                                  NewStringType::kNormal)
                                  .ToLocalChecked());
      return;
    }

    Local<String> retval = maybeStr.ToLocalChecked();
    args.GetReturnValue().Set(retval);

    return;
  }

  auto strLastUpdate = string_utils::actual_time2string();

  Person pers(strPersonID, strName, strSurname, strEmail, strLastUpdate);

  if (manager.AddItem(pers)) {
    str_ui_response = "The person ID: " + strPersonID + " was added.";
    str_ui_response +=
        "<br>--------------------------------------------------------";
    str_ui_response += "<br> Name          : " + strName;
    str_ui_response += "<br> Surname       : " + strSurname;
    str_ui_response += "<br> Email         : " + strEmail;
    str_ui_response += "<br> Last Update   : " + strLastUpdate;

    manager.Save(DATA_FILE);
  } else {
    str_ui_response =
        "<br> Error! The person ID [" + strPersonID + "] already exists!";
  }

  Local<String> retval;
  MaybeLocal<String> maybeRetVal =
      String::NewFromUtf8(isolate, str_ui_response.c_str());

  if (!maybeRetVal.ToLocal(&retval)) {
    isolate->ThrowException(String::NewFromUtf8(isolate,
                                                "Error creating string",
                                                NewStringType::kNormal)
                                .ToLocalChecked());
    return;
  }

  args.GetReturnValue().Set(retval);
}

void searchItem(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();

  if (args.Length() < 1 || !args[0]->IsString()) {
    isolate->ThrowException(
        String::NewFromUtf8(isolate, "First argument must be a string",
                            NewStringType::kNormal)
            .ToLocalChecked());
    return;
  }

  v8::String::Utf8Value search_string(
      isolate, args[0]->ToString(context).ToLocalChecked());
  std::string search_pid = std::string(*search_string);
  std::string str_ui_response;

  if (search_pid.empty()) {
    str_ui_response = "<br> No search criteria available.";
    MaybeLocal<String> maybeRetVal = String::NewFromUtf8(
        isolate, str_ui_response.c_str(), NewStringType::kNormal);
    Local<String> retval;
    if (!maybeRetVal.ToLocal(&retval)) {
      isolate->ThrowException(String::NewFromUtf8(isolate,
                                                  "Error creating string",
                                                  NewStringType::kNormal)
                                  .ToLocalChecked());
      return;
    }
    args.GetReturnValue().Set(retval);
    return;
  }

  auto results = manager.SearchItem(search_pid);
  if (nullptr == results) {
    str_ui_response =
        "<br> No result(s) found for person ID (" + search_pid + ")!";
  } else {
    str_ui_response = "Found the person ID: " + search_pid;
    str_ui_response +=
        "<br>--------------------------------------------------------";
    str_ui_response += "<br> Name          : " + results->getName();
    str_ui_response += "<br> Surname       : " + results->getSurname();
    str_ui_response += "<br> Email         : " + results->getEmail();
    str_ui_response += "<br> Last Update   : " + results->getLastUpdate();
  }


  Local<String> retval;
  MaybeLocal<String> maybeRetVal =
      String::NewFromUtf8(isolate, str_ui_response.c_str());

  if (!maybeRetVal.IsEmpty()) {
    retval = maybeRetVal.ToLocalChecked();
    args.GetReturnValue().Set(retval);
  } else {
    isolate->ThrowException(String::NewFromUtf8(isolate,
                                                "String creation failed",
                                                NewStringType::kNormal)
                                .ToLocalChecked());
  }
}

void deleteItem(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  Local<Context> context = isolate->GetCurrentContext();

  if (args.Length() < 1 || !args[0]->IsString()) {
    isolate->ThrowException(
        String::NewFromUtf8(isolate, "First argument must be a string",
                            NewStringType::kNormal)
            .ToLocalChecked());
    return;
  }

  v8::String::Utf8Value personID(isolate,
                                 args[0]->ToString(context).ToLocalChecked());
  std::string strPersonID = std::string(*personID);
  std::string str_ui_response;

  if (strPersonID.empty()) {
    str_ui_response = "<br> Error! Person ID is empty.";
  } else if (manager.DeleteItem(strPersonID)) {
    str_ui_response = "<br> The person ID: " + strPersonID + " was deleted!";
    manager.Save(DATA_FILE);
  } else {
    str_ui_response = "<br> The person ID: " + strPersonID + " does not exist!";
  }

  Local<String> retval;
  MaybeLocal<String> maybeRetVal = String::NewFromUtf8(
      isolate, str_ui_response.c_str(), NewStringType::kNormal);
  if (!maybeRetVal.ToLocal(&retval)) {
    isolate->ThrowException(String::NewFromUtf8(isolate,
                                                "String creation failed",
                                                NewStringType::kNormal)
                                .ToLocalChecked());
    return;
  }
  args.GetReturnValue().Set(retval);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "initEngine", initEngine);
  NODE_SET_METHOD(exports, "addItem", addItem);
  NODE_SET_METHOD(exports, "deleteItem", deleteItem);
  NODE_SET_METHOD(exports, "searchItem", searchItem);
}

NODE_MODULE(engine_addon, init)

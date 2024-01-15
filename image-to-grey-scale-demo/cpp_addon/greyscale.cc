#include <node.h>
#include <opencv2/opencv.hpp>

using namespace v8;
using namespace cv;

// Function to convert an image to grayscale
void ConvertToGrayscale(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed
  if (args.Length() < 2) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, "Wrong number of arguments")
            .ToLocalChecked()));
    return;
  }

  // Check the argument types
  if (!args[0]->IsString() || !args[1]->IsFunction()) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, "Wrong arguments").ToLocalChecked()));
    return;
  }

  v8::String::Utf8Value str(isolate, args[0]);
  std::string inputPath(*str);

  // Read the image file
  Mat image = imread(inputPath);
  if (image.empty()) {
    isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, "Could not read the image")
            .ToLocalChecked()));
    return;
  }

  // Convert the image to grayscale
  Mat grayImage;
  cvtColor(image, grayImage, COLOR_BGR2GRAY);

  // Save the processed image
  std::string outputPath = inputPath + "_gray.jpg";
  bool writeSuccess = imwrite(outputPath, grayImage);

  Local<Function> callback = Local<Function>::Cast(args[1]);
  Local<Context> context = isolate->GetCurrentContext();

  if (!writeSuccess) {
    // If imwrite failed, invoke the callback with an error
    Local<Value> err =
        v8::String::NewFromUtf8(isolate, "Failed to write processed image")
            .ToLocalChecked();
    const unsigned argc = 1;
    Local<Value> argv[argc] = {err};
    callback->Call(context, context->Global(), argc, argv);
  } else {
    // If imwrite succeeded, invoke the callback with the output path
    Local<Value> argv[2] = {
        Null(isolate),  // No error
        v8::String::NewFromUtf8(isolate, outputPath.c_str())
            .ToLocalChecked()  // Processed image path
    };
    callback->Call(context, context->Global(), 2, argv);
  }
}

void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "processImage", ConvertToGrayscale);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

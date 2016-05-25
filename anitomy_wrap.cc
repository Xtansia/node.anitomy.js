#include <nan.h>

NAN_METHOD(Hello) {
  info.GetReturnValue().Set(Nan::New("Hello World!").ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, Nan::New("hello").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Hello)->GetFunction());
}

NODE_MODULE(anitomy, Init)
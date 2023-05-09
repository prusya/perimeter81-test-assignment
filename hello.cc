#include <napi.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

Napi::Number ReadValue(const Napi::CallbackInfo& info) {
	using namespace std;
	
	cout << "ReadValue called\n";
	
	Napi::Env env = info.Env();
	string p = string(getenv("HOME")).append(+ "/Desktop/counter.json");
    ifstream ifs(p);
    string str(istreambuf_iterator<char>{ifs}, {});    
    Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
	Napi::Function parse = json.Get("parse").As<Napi::Function>();
	Napi::Object data = parse.Call(json, { Napi::String::New(env, str) }).As<Napi::Object>();
	Napi::Number val = data.Get("counterStartingPoint").As<Napi::Number>();
    
	return val;
}

Napi::Number SaveValue(const Napi::CallbackInfo& info) {
	using namespace std;
	
	cout << "SaveValue called\n";
	
	Napi::Env env = info.Env();
	Napi::Number val = info[0].As<Napi::Number>();
	Napi::Object obj = Napi::Object::New(env);
	obj.Set("counterStartingPoint", val);
	Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
  	Napi::Function stringify = json.Get("stringify").As<Napi::Function>();
  	Napi::String data = stringify.Call(json, { obj }).As<Napi::String>();
  	
  	cout << string(data) << '\n';
  	
  	string p = string(getenv("HOME")).append(+ "/Desktop/counter.json");
  	ofstream ofs;
  	ofs.open(p);
  	ofs << string(data);
  	ofs.close();
	
	return Napi::Number::New(env, 5);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {     
  exports["readValue"] = Napi::Function::New(env, ReadValue); 
  exports["saveValue"] = Napi::Function::New(env, SaveValue);    
  return exports;
}

NODE_API_MODULE(json, Init)

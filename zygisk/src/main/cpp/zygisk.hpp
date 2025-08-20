#pragma once
#include <jni.h>

namespace zygisk {

class Api {
public:
    virtual ~Api() = default;
    virtual JNIEnv* env() = 0;
};

struct AppSpecializeArgs {
    const char* nice_name;
    const char* app_data_dir;
};

class ModuleBase {
public:
    virtual ~ModuleBase() = default;
    virtual void onLoad(Api* api, JNIEnv* env) {}
    virtual void preAppSpecialize(AppSpecializeArgs* args) {}
    virtual void postAppSpecialize(const AppSpecializeArgs* args) {}
};

}

#define REGISTER_ZYGISK_MODULE(class_name) \
extern "C" __attribute__((visibility("default"))) void zygisk_module_register(zygisk::ModuleBase** ret) { \
    static class_name instance; \
    *ret = &instance; \
}
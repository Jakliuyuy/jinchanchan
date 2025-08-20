#include <jni.h>
#include <string>
#include <android/log.h>
#include "zygisk.hpp"

#define LOG_TAG "JCC120Spoof"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static std::string target_pkg = "com.tencent.jkchess";

// Helper: set static final String field via reflection
static void setStaticBuildField(JNIEnv* env, const char* name, const char* value) {
    jclass build = env->FindClass("android/os/Build");
    if (!build) { LOGE("Build class not found"); return; }

    jfieldID fid = env->GetStaticFieldID(build, name, "Ljava/lang/String;");
    if (!fid) { LOGE("Field %s not found", name); return; }

    jstring jval = env->NewStringUTF(value);
    env->SetStaticObjectField(build, fid, jval);
    env->DeleteLocalRef(jval);
    env->DeleteLocalRef(build);
}

// Helper: for Build.FINGERPRINT (same as above but spelled here for clarity)
static void setFingerprint(JNIEnv* env, const char* fp) {
    setStaticBuildField(env, "FINGERPRINT", fp);
}

// Helper: set private static final fields by clearing final (works for String fields on Android 8-14)
static void spoofBuildAll(JNIEnv* env,
                          const char* brand,
                          const char* manuf,
                          const char* model,
                          const char* device,
                          const char* product,
                          const char* board,
                          const char* fingerprint) {
    setStaticBuildField(env, "BRAND", brand);
    setStaticBuildField(env, "MANUFACTURER", manuf);
    setStaticBuildField(env, "MODEL", model);
    setStaticBuildField(env, "DEVICE", device);
    setStaticBuildField(env, "PRODUCT", product);
    setStaticBuildField(env, "BOARD", board);
    setFingerprint(env, fingerprint);
    LOGI("Applied spoof: %s/%s %s fp=%s", brand, manuf, model, fingerprint);
}

class JccModule : public zygisk::ModuleBase {
public:
    void onLoad(Api* api, JNIEnv* env) override {
        this->api_ = api;
        this->env_ = env;
        LOGI("Module loaded");
    }

    void preAppSpecialize(AppSpecializeArgs* args) override {
        // Nothing here
    }

    void postAppSpecialize(const AppSpecializeArgs* args) override {
        const char* pkg = args->nice_name;
        if (!pkg) return;
        std::string name(pkg);
        if (name != target_pkg) return;

        LOGI("Target app detected: %s", pkg);

        // Read assets/config.json packaged in module
        // We'll pack values at compile time for simplicity
        // Defaults to NX709S; can be toggled by replacing strings via config
        const char* brand = getenv("JCC_BRAND");
        const char* manuf = getenv("JCC_MANUFACTURER");
        const char* model = getenv("JCC_MODEL");
        const char* device = getenv("JCC_DEVICE");
        const char* product = getenv("JCC_PRODUCT");
        const char* board = getenv("JCC_BOARD");
        const char* fp = getenv("JCC_FINGERPRINT");

        // Fallback hardcoded NX709S values if env not provided
        if (!brand)  brand  = "nubia";
        if (!manuf)  manuf  = "nubia";
        if (!model)  model  = "NX709S";
        if (!device) device = "NX709S";
        if (!product)product = "NX709S";
        if (!board)  board  = "kona";
        if (!fp)     fp     = "nubia/NX709S/NX709S:13/TKQ1.221013.002/20230914:user/release-keys";

        spoofBuildAll(api_->env(), brand, manuf, model, device, product, board, fp);
    }

private:
    Api* api_;
    JNIEnv* env_;
};

static JccModule g_module;

REGISTER_ZYGISK_MODULE(JccModule)
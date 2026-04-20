#include <jni.h>
#include "escpos_engine.h"

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_example_printer_PrinterBridge_buildPrintJob(
        JNIEnv* env,
        jobject,
        jintArray pixels,
        jint width,
        jint height
) {
    jint* raw = env->GetIntArrayElements(pixels, nullptr);
    jsize len = env->GetArrayLength(pixels);

    std::span<const int32_t> span(raw, len);

    auto result = EscPosCore::prepareRasterData(span, width, height);

    env->ReleaseIntArrayElements(pixels, raw, 0);

    jbyteArray output = env->NewByteArray(result.size());
    env->SetByteArrayRegion(
            output,
            0,
            result.size(),
            reinterpret_cast<const jbyte*>(result.data())
    );

    return output;
}
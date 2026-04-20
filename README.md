Här är en ren och professionell README.md på engelska som tydliggör att motorn fungerar både som ett fristående bibliotek och som en Android-modul.

ESC/POS C++ Engine
This is a high-performance C++20 engine for ESC/POS thermal printing. It is designed as a standalone C++ library for any C++ project, while also providing an optional Android JNI bridge for mobile integration.

Usage
1. Pure C++ Projects
   The core logic in escpos_engine.cpp is platform-agnostic and has no external dependencies.

Include escpos_engine.h in your project.

Call the engine using std::span or std::vector for image data.

C++
#include "escpos_engine.h"

// Example call
auto printData = EscPosCore::prepareRasterData(pixelSpan, width, height);
2. Android Integration
   Copy the source files to src/main/cpp/.

Link the provided CMakeLists.txt in your build.gradle.

Use the Kotlin interface. Note: The Kotlin file must reside in the com.example.printer package to match the JNI signatures.

Kotlin
// In your Android Activity/Service
val pixels = IntArray(width * height)
bitmap.getPixels(pixels, 0, width, 0, 0, width, height)

val printData = PrinterBridge.buildPrintJob(pixels, width, height)
Technical Specifications
Language Standard: C++20 (requires std::span).

Format: Supports GS v 0 (Raster bit image) commands.

Binarization: Built-in luminance-based conversion (Threshold: 128).

Architecture
escpos_engine.cpp: Core printing logic (Universal).

jni_bridge.cpp: JNI translation layer (Optional for Android).

PrinterBridge.kt: Kotlin API (Optional for Android).
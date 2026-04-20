package com.example.printer

object PrinterBridge {
    external fun buildPrintJob(
        pixels: IntArray,
        width: Int,
        height: Int
    ): ByteArray

    init {
        System.loadLibrary("printerengine")
    }
}
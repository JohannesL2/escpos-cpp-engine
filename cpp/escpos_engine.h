#pragma once
#include <vector>
#include <cstdint>
#include <span>

namespace EscPosCore {
    std::vector<uint8_t> buildPrintJob(
            std::span<const int32_t> pixels,
            int width,
            int height
    );
}
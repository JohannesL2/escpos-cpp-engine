#include <vector>
#include <cstdint>
#include <span>
#include <iostream>
#include <cstdio>

// ================== //
// ESC POS C++ ENGINE //

namespace EscPosCore {


    std::vector<uint8_t> prepareRasterData(
            std::span<const int32_t> pixels,
            int width,
            int height
            );
    {
        const int widthBytes = (width + 7) / 8;
        std::vector<uint8_t> data;

        data.reserve(8 + (widthBytes * height));

        data.push_back(0x1D);
        data.push_back(0x76);
        data.push_back(0x30);
        data.push_back(0);

        data.push_back(static_cast<uint8_t>(widthBytes % 256));
        data.push_back(static_cast<uint8_t>(widthBytes / 256));

        data.push_back(static_cast<uint8_t>(height % 256));
        data.push_back(static_cast<uint8_t>(height / 256));

        for (int y = 0; y < height; ++y) {
            for (int xByte = 0; xByte < widthBytes; ++xByte) {
                uint8_t currentByte = 0;
                
                // Convert pixels to byte
                for (int bit = 0; bit < 8; ++bit) {
                    int pxX = xByte * 8 + bit;
                    
                    if (pxX < width) {
                        // Get pixels and convert to binary
                        int32_t p = pixels[y * width + pxX];

                        int r = (p >> 16) & 0xFF;
                        int g = (p >> 8) & 0xFF;
                        int b = p & 0xFF;
                        
                        if ((r * 0.299 + g * 0.587 + b * 0.114) < 128) {
                            // if value is under 128 consider it as black
                            // ESC/POS använder Most Significant Bit (MSB) först.
                            currentByte |= (1 << (7 - bit));
                        }
                    }
                }
                data.push_back(currentByte);
            }
        }
        
        return data;
    }

    /**
     * Stop print and cut paper
     */
    std::vector<uint8_t> getPaperCutCommand() {
        // 'GS V 66 0' (Feed and paper cut)
        return {0x1D, 0x56, 66, 0};
    }
}

#ifndef __ANDROID__
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "ESC/POS C++ Engine - LOCAL TEST" << std::endl;
    std::cout << "========================================" << std::endl;

    //Test bitmap chess pattern
    const int width = 16;
    const int height = 4;
    std::vector<int32_t> mockPixels(width * height);
    
    std::cout << "\nGenererar testbild (schackmönster)..." << std::endl;
    for (int i = 0; i < width * height; ++i) {
        // Varannan pixel svart (0x000000), varannan vit (0xFFFFFF).
        mockPixels[i] = (i % 2 == 0) ? 0x000000 : 0xFFFFFF;
    }

    // 2. Anropa motorn.
    std::cout << "Anropar motorn med prepareRasterData..." << std::endl;
    auto resultData = EscPosCore::prepareRasterData(mockPixels, width, height);

    // 3. Verifiera resultatet.
    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "RESULTAT" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total storlek: " << resultData.size() << " bytes." << std::endl;
    
    // Header-koll
    std::cout << "Header: ";
    for (int i = 0; i < 8; ++i) printf("%02X ", resultData[i]);
    std::cout << std::endl;

    std::cout << "\nASCII-förhandsvisning av binariseringen:" << std::endl;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Beräkna vilken byte och bit denna pixel motsvarar i resultData.
            int widthBytes = (width + 7) / 8;
            int byteIndex = 8 + (y * widthBytes) + (x / 8);
            int bitIndex = 7 - (x % 8);
            
            bool isBlack = (resultData[byteIndex] & (1 << bitIndex)) != 0;
            std::cout << (isBlack ? "#" : ".");
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;

    return 0;
}
#endif
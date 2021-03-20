#pragma once
#include <cstdint>

class NetworkHelper {
public:


    static uint16_t ConvertU16(const uint8_t* data, int64_t off) {
        return data[off] | (data[off + 1] << 8);
    }

    static uint32_t ConvertU32(const uint8_t* data, int64_t off) {
        int32_t res = data[off] | (data[off + 1] << 8) |  (data[off + 2] << 16) | (data[off + 3] << 24);
        return res;
    }

    static void ConvertU32(uint8_t* data, int64_t off, uint32_t val) {
        data[off] = val & 0xFF;
        data[off + 1] = (val >> 8) & 0xFF;
        data[off + 2] = (val >> 16) & 0xFF;
        data[off + 3] = (val >> 24) & 0xFF;
    }

    static void ConvertU16(uint8_t* data, int64_t off, uint16_t val) {
        data[off] = val & 0xFF;
        data[off + 1] = (val >> 8) & 0xFF;
    }
};

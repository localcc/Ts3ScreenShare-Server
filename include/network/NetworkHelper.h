#pragma once
#include <cstdint>
#include <stdexcept>
#include <enet.h>

class NetworkHelper {
public:

    enum EventTypes {
        EVENT_TYPE_LOGIN,
        EVENT_TYPE_FRAME,
        EVENT_TYPE_START_WATCHING,
        EVENT_TYPE_STOP_WATCHING,
        EVENT_TYPE_CREATE_WINDOW,
        EVENT_TYPE_START_STREAM
    };

    static ENetHost* EnetHostCreateEx(int16_t port) {
        ENetAddress address{};
        address.host = ENET_HOST_ANY;
        address.port = port;

        ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
        if(server == nullptr) {
            throw std::runtime_error("Failed to create host!");
        }

        return server;
    }

    static ENetHost* EnetHostCreateEx() {
        ENetHost* server = enet_host_create(nullptr, 1, 2, 0, 0);
        if(server == nullptr) {
            throw std::runtime_error("Failed to create host!");
        }
        return server;
    }

    static uint16_t ConvertU16(const uint8_t* data, int64_t off) {
        return data[off] | (data[off + 1] << 8);
    }

    static uint32_t ConvertU32(const uint8_t* data, int64_t off) {
        uint32_t res = data[off] | (data[off + 1] << 8) |  (data[off + 2] << 16) | (data[off + 3] << 24);
        return res;
    }

    static int32_t Convert32(const uint8_t* data, int64_t off) {
        int32_t res = data[off] | (data[off + 1] << 8) |  (data[off + 2] << 16) | (data[off + 3] << 24);
        return res;
    }


    static int64_t Convert64(const uint8_t* data, int64_t off) {
        int64_t res = data[off] | (data[off + 1] << 8) | (data[off + 2] << 16) | (data[off + 3] << 24) |
                (static_cast<int64_t>(data[off + 4]) << 32) | (static_cast<int64_t>(data[off + 5]) << 40) | (static_cast<int64_t>(data[off + 6]) << 48) |
                (static_cast<int64_t>(data[off + 7]) << 56);
        return res;
    }

    static void Convert64(uint8_t* data, int64_t off, int64_t val) {
        for(int i = 0; i < 8; i++) {
            data[i + off] = (val >> (8 * i)) & 0xFF;
        }
    }


    static void ConvertU32(uint8_t* data, int64_t off, uint32_t val) {
        data[off] = val & 0xFF;
        data[off + 1] = (val >> 8) & 0xFF;
        data[off + 2] = (val >> 16) & 0xFF;
        data[off + 3] = (val >> 24) & 0xFF;
    }

    static void Convert32(uint8_t* data, int64_t off, int32_t val) {
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

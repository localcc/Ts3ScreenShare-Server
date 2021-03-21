#pragma once
#include <network/NetworkHelper.h>
#include <network/packets/IPacket.h>

class CreateWindowPacket : public IPacket {
public:
    CreateWindowPacket(uint16_t clientId, uint32_t width, uint32_t height) {
        this->clientId = clientId;
        this->width = width;
        this->height = height;
    }

    CreateWindowPacket(const uint8_t* data) {
        this->clientId = NetworkHelper::ConvertU16(data, 1);
        this->width = NetworkHelper::ConvertU32(data, 3);
        this->height = NetworkHelper::ConvertU32(data, 7);
    }

    void write(uint8_t* data) override {
        data[0] = NetworkHelper::EVENT_TYPE_CREATE_WINDOW;
        NetworkHelper::ConvertU16(data, 1, this->clientId);
        NetworkHelper::ConvertU32(data, 3, this->width);
        NetworkHelper::ConvertU32(data, 7, this->height);
    }

    const uint64_t getSize() override {
        return 11;
    }

public:
    uint16_t clientId;
    uint32_t width;
    uint32_t height;
};

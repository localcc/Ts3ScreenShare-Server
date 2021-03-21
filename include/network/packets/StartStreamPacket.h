#pragma once
#include <network/NetworkHelper.h>
#include <network/packets/IPacket.h>

class StartStreamPacket : public IPacket {
public:
    StartStreamPacket(uint32_t width, uint32_t height, uint32_t pix_fmt) {
        this->width = width;
        this->height = height;
        this->pix_fmt = pix_fmt;
    }

    StartStreamPacket(const uint8_t* data) {
        this->width = NetworkHelper::ConvertU32(data, 1);
        this->height = NetworkHelper::ConvertU32(data, 5);
        this->pix_fmt = NetworkHelper::ConvertU32(data, 9);
    }

    void write(uint8_t* data) override {
        data[0] = NetworkHelper::EVENT_TYPE_START_STREAM;
        NetworkHelper::ConvertU32(data, 1, this->width);
        NetworkHelper::ConvertU32(data, 5, this->height);
        NetworkHelper::ConvertU32(data, 9, this->pix_fmt);
    }


    const uint64_t getSize() override {
        return 13;
    }

public:
    uint32_t width;
    uint32_t height;
    uint32_t pix_fmt;
};

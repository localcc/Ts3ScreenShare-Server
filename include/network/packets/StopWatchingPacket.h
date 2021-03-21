#pragma once
#include <network/NetworkHelper.h>
#include <network/packets/IPacket.h>

class StopWatchingPacket : public IPacket {
public:
    StopWatchingPacket(uint16_t clientId) {
        this->clientId = clientId;
    }

    StopWatchingPacket(const uint8_t* data) {
        this->clientId = NetworkHelper::ConvertU16(data, 1);
    }

    void write(uint8_t* data) override {
        data[0] = NetworkHelper::EVENT_TYPE_STOP_WATCHING;
        NetworkHelper::ConvertU16(data, 1, this->clientId);
    }

    const uint64_t getSize() override {
        return 3;
    }

public:
    uint16_t clientId;
};

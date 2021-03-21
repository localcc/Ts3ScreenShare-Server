#pragma once
#include <network/NetworkHelper.h>
#include <network/packets/IPacket.h>

class StartWatchingPacket : public IPacket {
public:
    StartWatchingPacket(uint16_t clientID) {
        this->clientId = clientID;
    }

    StartWatchingPacket(const uint8_t* data) {
        this->clientId = NetworkHelper::ConvertU16(data, 1);
    }

    void write(uint8_t* data) override {
        data[0] = NetworkHelper::EVENT_TYPE_START_WATCHING;
        NetworkHelper::ConvertU16(data, 1, this->clientId);
    }


    const uint64_t getSize() override {
        return 3;
    }

public:
    uint16_t clientId;
};

#pragma once
#include <network/NetworkHelper.h>
#include <network/packets/IPacket.h>

class LoginPacket : public IPacket {
public:
    LoginPacket(uint16_t clientId) {
        this->clientId = clientId;
    }

    LoginPacket(const uint8_t* data) {
        this->clientId = NetworkHelper::ConvertU16(data, 1);
    }

    void write(uint8_t* data) override {
        data[0] = NetworkHelper::EVENT_TYPE_LOGIN;
        NetworkHelper::ConvertU16(data, 1, this->clientId);
    }

    const uint64_t getSize() override {
        return 3;
    }

public:
    uint16_t clientId;
};

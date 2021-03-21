#pragma once
#include <cstdint>

class IPacket {
public:
    virtual void write(uint8_t* data) = 0;
    virtual const uint64_t getSize() = 0;
};

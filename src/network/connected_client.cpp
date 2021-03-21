#include <network/connected_client.h>

connected_client::connected_client(std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)> peer) : peer(std::move(peer)) {
}

int32_t connected_client::write(const uint8_t* data, int64_t size) {
    ENetPacket* packet = enet_packet_create(data, size, 0);
    int32_t res = enet_peer_send(this->peer.get(), 1, packet);
    enet_packet_destroy(packet);
    return res;
}

void connected_client::addWatchingMe(uint16_t id)
{
    this->watching_me.push_back(id);
}

void connected_client::removeWatchingMe(uint16_t id)
{
    std::remove(this->watching_me.begin(), this->watching_me.end(), id);
}

void connected_client::addWatchedByMe(uint16_t id)
{
    this->watched_by_me.push_back(id);
}

void connected_client::removeWatchedByMe(uint16_t id)
{
    std::remove(this->watched_by_me.begin(), this->watched_by_me.end(), id);
}

void connected_client::setResolution(uint32_t width, uint32_t height)
{
    this->width = width;
    this->height = height;
}

std::tuple<uint32_t, uint32_t> connected_client::getResolution() const
{
    return {this->width, this->height};
}

const std::vector<uint16_t> connected_client::getWatchingMe() const
{
    return this->watching_me;
}

const std::vector<uint16_t> connected_client::getWatchedByMe() const
{
    return this->watched_by_me;
}

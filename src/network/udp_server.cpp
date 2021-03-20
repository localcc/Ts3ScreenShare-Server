#include <network/udp_server.h>
#include <iostream>
#include <network/NetworkHelper.h>

udp_server::udp_server(int16_t port) :
    server(NetworkHelper::EnetHostCreateEx(port), &enet_host_destroy) {

}

void udp_server::run() {
    ENetEvent event;
    this->running = true;
    while(enet_host_service(this->server.get(), &event, 0) >= 0 && this->running) {
        switch(event.type) {
            case ENET_EVENT_TYPE_CONNECT: {

                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                auto* const packet = event.packet;
                const auto find = this->callbacks.find(packet->data[0]);
                if(find != this->callbacks.end()) {
                    find->second(this, event);
                }

                enet_packet_destroy(event.packet);

                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                this->handle_disconnect(event);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
                this->handle_disconnect(event);
                break;
            }
            case ENET_EVENT_TYPE_NONE:
                break;
        }
    }
}

void udp_server::handle_login(udp_server* server, ENetEvent event) {
    auto* const packet = event.packet;
    const auto clientID = NetworkHelper::ConvertU16(packet->data, 0);
    if(server->connectedClients.find(clientID) != server->connectedClients.end()) {
        return;
    }
    event.peer->data = reinterpret_cast<void*>(clientID);

    std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)> peer(event.peer, &enet_peer_reset);
    server->connectedClients.emplace(clientID, std::make_unique<connected_client>(std::move(peer)));
}

void udp_server::handle_frame(udp_server* server, ENetEvent event) {
    const auto clientID = static_cast<uint16_t>(reinterpret_cast<uintptr_t>(event.peer->data));
    const auto client = server->connectedClients.find(clientID);
    if(client == server->connectedClients.end()) return;



}

void udp_server::handle_start_watch_stream(udp_server* server, ENetEvent event) {
    const auto clientID = static_cast<uint16_t>(reinterpret_cast<uintptr_t>(event.peer->data));
    const auto client = server->connectedClients.find(clientID);
    if(client == server->connectedClients.end()) return;

    auto* const packet = event.packet;
    const auto watchedID = NetworkHelper::ConvertU16(packet->data, 0);
    const auto watchedClient = server->connectedClients.find(watchedID);
    if(watchedClient == server->connectedClients.end()) {
        return;
    }

    client->second->addWatchedByMe(watchedID);
    watchedClient->second->addWatchingMe(clientID);
}

void udp_server::handle_stop_watch_stream(udp_server* server, ENetEvent event) {
    const auto clientID = static_cast<uint16_t>(reinterpret_cast<uintptr_t>(event.peer->data));
    const auto client = server->connectedClients.find(clientID);
    if(client == server->connectedClients.end()) return;

    auto* const packet = event.packet;
    const auto watchedID = NetworkHelper::ConvertU16(packet->data, 0);
    const auto watchedClient = server->connectedClients.find(watchedID);
    if(watchedClient == server->connectedClients.end()) {
        return;
    }

    client->second->removeWatchedByMe(watchedID);
    watchedClient->second->removeWatchingMe(clientID);
}

void udp_server::handle_disconnect(ENetEvent event) {
    const auto clientID = static_cast<uint16_t>(reinterpret_cast<uintptr_t>(event.peer->data));
    const auto client = this->connectedClients.find(clientID);
    if(client == this->connectedClients.end()) return;

    for(const auto& peer : this->connectedClients) {
        peer.second->removeWatchedByMe(clientID);
        peer.second->removeWatchingMe(clientID);
    }

    this->connectedClients.erase(clientID);
}

udp_server::~udp_server() {
    this->running = false;
}

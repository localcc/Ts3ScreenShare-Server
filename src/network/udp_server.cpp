#include <network/udp_server.h>
#include <iostream>
#include <network/NetworkHelper.h>
#include <network/packets/LoginPacket.h>
#include <network/packets/CreateWindowPacket.h>
#include <network/packets/StartWatchingPacket.h>
#include <network/packets/StopWatchingPacket.h>
#include <network/packets/StartStreamPacket.h>
#include <chrono>

udp_server::udp_server(int16_t port)
    //server(NetworkHelper::EnetHostCreateEx(port), &enet_host_destroy) {
{
    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = port;
    this->server = enet_host_create(&address, 32, 2, 0, 0);
}

void udp_server::run() {
    ENetEvent event;
    this->running = true;
    while(enet_host_service(this->server, &event, 0) >= 0 && this->running) {
        switch(event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << "Connected!" << std::endl;
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                auto* const packet = event.packet;
                std::cout << "Event: " << static_cast<uint32_t>(packet->data[0]) << std::endl;
                switch(packet->data[0]) {
                    case NetworkHelper::EVENT_TYPE_LOGIN:
                        this->handle_login(event);
                        break;
                    case NetworkHelper::EVENT_TYPE_FRAME: {
                            const auto startTime = std::chrono::system_clock::now();
                            this->handle_frame(event);
                            const auto endTime = std::chrono::system_clock::now();
                            std::cout << "Time to handle frame: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << std::endl;
                            break;
                    }
                    case NetworkHelper::EVENT_TYPE_START_WATCHING:
                        this->handle_start_watch_stream(event);
                        break;
                    case NetworkHelper::EVENT_TYPE_STOP_WATCHING:
                        this->handle_stop_watch_stream(event);
                        break;
                    case NetworkHelper::EVENT_TYPE_START_STREAM:
                        this->handle_start_stream(event);
                    default:
                        break;
                }
                std::cout << "Destroying packet" << std::endl;
                enet_packet_destroy(event.packet);

                std::cout << "Destroyed packet" << std::endl;
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
    std::cout << "Cycle end" << std::endl;
}

void udp_server::handle_login(ENetEvent event) {
    auto* const packet = new LoginPacket(event.packet->data);
    const auto clientID = packet->clientId;
    if(this->connectedClients.find(clientID) != this->connectedClients.end()) {
        return;
    }
    event.peer->data = reinterpret_cast<void*>(clientID);

    std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)> peer(event.peer, &enet_peer_reset);
    this->connectedClients.emplace(clientID, std::make_unique<connected_client>(std::move(peer)));
}

void udp_server::handle_frame(ENetEvent event) {
    const auto clientID = static_cast<uint16_t>(reinterpret_cast<uintptr_t>(event.peer->data));
    const auto client = this->connectedClients.find(clientID);
    if(client == this->connectedClients.end()) return;

    //no packet here for faster code execution
    const auto watchingMe = client->second->getWatchingMe();
    for(const auto& watchingId : watchingMe) {
        const auto watching = this->connectedClients.find(watchingId);
        if(watching == this->connectedClients.end()) {
            client->second->removeWatchingMe(watchingId);
            client->second->removeWatchedByMe(watchingId);
        }

        auto* const nData = new uint8_t[event.packet->dataLength + 3];
        nData[0] = 1; // frame packet
        NetworkHelper::ConvertU16(nData, 1, clientID);
        std::move(event.packet->data, event.packet->data + event.packet->dataLength, nData + 2);

        watching->second->write(nData, event.packet->dataLength + 3);

        delete[] nData;
    }

}

void udp_server::handle_start_watch_stream(ENetEvent event) {
    const auto clientID = static_cast<uint16_t>(reinterpret_cast<uintptr_t>(event.peer->data));
    const auto client = this->connectedClients.find(clientID);
    if(client == this->connectedClients.end()) return;

    auto* const packet = new StartWatchingPacket(event.packet->data);
    const auto watchedID = packet->clientId;
    const auto watchedClient = this->connectedClients.find(watchedID);
    if(watchedClient == this->connectedClients.end()) {
        return;
    }

    if(std::find(client->second->getWatchedByMe().begin(), client->second->getWatchedByMe().end(), watchedID) == client->second->getWatchedByMe().end()) {
        client->second->addWatchedByMe(watchedID);

        const auto resolution = watchedClient->second->getResolution();
/*
        const auto createWindowPacket = new CreateWindowPacket(clientID, std::get<0>(resolution), std::get<1>(resolution));
        auto* bytes = new uint8_t[createWindowPacket->getSize()];
        createWindowPacket->write(bytes);
        client->second->write(bytes, createWindowPacket->getSize());
        delete[] bytes;*/

    }
    if(std::find(watchedClient->second->getWatchedByMe().begin(), watchedClient->second->getWatchingMe().end(), clientID) == watchedClient->second->getWatchingMe().end()) {
        watchedClient->second->addWatchingMe(clientID);
    }

}

void udp_server::handle_stop_watch_stream(ENetEvent event) {
    const auto clientID = static_cast<uint16_t>(reinterpret_cast<uintptr_t>(event.peer->data));
    const auto client = this->connectedClients.find(clientID);
    if(client == this->connectedClients.end()) return;

    auto* const packet = new StopWatchingPacket(event.packet->data);
    const auto watchedID = packet->clientId;
    const auto watchedClient = this->connectedClients.find(watchedID);
    if(watchedClient == this->connectedClients.end()) {
        return;
    }

    client->second->removeWatchedByMe(watchedID);
    watchedClient->second->removeWatchingMe(clientID);
}

void udp_server::handle_start_stream(ENetEvent event) {
    const auto clientID = static_cast<uint16_t>(reinterpret_cast<uintptr_t>(event.peer->data));
    const auto client = this->connectedClients.find(clientID);
    if(client == this->connectedClients.end()) return;

    auto* const packet = new StartStreamPacket(event.packet->data);

    client->second->setResolution(packet->width, packet->height);
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

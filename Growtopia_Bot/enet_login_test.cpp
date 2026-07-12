#include <enet/enet.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cstring>

static void hexdump(const uint8_t* data, size_t len) {
    const size_t bytes_per_line = 16;
    for (size_t i = 0; i < len; i += bytes_per_line) {
        std::cout << std::setw(4) << std::setfill('0') << std::hex << i << ": ";
        for (size_t j = 0; j < bytes_per_line; ++j) {
            if (i + j < len) std::cout << std::setw(2) << (int)data[i + j] << " ";
            else std::cout << "   ";
        }
        std::cout << " ";
        for (size_t j = 0; j < bytes_per_line; ++j) {
            if (i + j < len) {
                char c = (char)data[i + j];
                std::cout << (isprint((unsigned char)c) ? c : '.');
            }
        }
        std::cout << std::dec << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <host> <port> [type] [payload-as-text]" << std::endl;
        std::cerr << "Example: ./enet_login_test growtopia1.com 1704 0 \"GT|login|user|pass\"" << std::endl;
        return 1;
    }

    const char* host = argv[1];
    int port = std::stoi(argv[2]);
    int type = 0;
    std::string payload;

    if (argc >= 4) type = std::stoi(argv[3]);
    if (argc >= 5) payload = argv[4];

    if (enet_initialize() != 0) {
        std::cerr << "ENet initialization failed." << std::endl;
        return 1;
    }
    atexit(enet_deinitialize);

    ENetHost* client = enet_host_create(NULL /* create a client host */,
                                        1 /* only allow 1 outgoing connection */, 
                                        2 /* allow up 2 channels to be used, 0 and 1 */, 
                                        0 /* downstream bandwidth */, 
                                        0 /* upstream bandwidth */);
    if (!client) {
        std::cerr << "Failed to create ENet client host." << std::endl;
        return 1;
    }

    ENetAddress address;
    if (enet_address_set_host(&address, host) < 0) {
        std::cerr << "Failed to resolve host: " << host << std::endl;
        enet_host_destroy(client);
        return 1;
    }
    address.port = port;

    ENetPeer* peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
        std::cerr << "No available peers for initiating a connection." << std::endl;
        enet_host_destroy(client);
        return 1;
    }

    std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected to server." << std::endl;
    } else {
        enet_peer_reset(peer);
        std::cerr << "Connection to server timed out or failed." << std::endl;
        enet_host_destroy(client);
        return 1;
    }

    // Build packet in same style as project helpers: 4-byte int type + payload
    std::vector<uint8_t> data;
    data.resize(4 + payload.size());
    // little-endian int
    std::memcpy(data.data(), &type, 4);
    if (!payload.empty()) std::memcpy(data.data() + 4, payload.data(), payload.size());

    ENetPacket* packet = enet_packet_create(nullptr, data.size() + 1, ENET_PACKET_FLAG_RELIABLE);
    if (!packet) {
        std::cerr << "Failed to create ENet packet." << std::endl;
        enet_peer_reset(peer);
        enet_host_destroy(client);
        return 1;
    }
    std::memcpy(packet->data, data.data(), data.size());
    packet->data[data.size()] = 0; // zero-terminate for safety if the server expects strings

    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);

    std::cout << "Packet sent (type=" << type << ", payload_len=" << payload.size() << ")." << std::endl;

    // Receive loop: print any incoming packets for up to 10 seconds
    const int receiveTimeoutMs = 10000; // 10s
    auto start = std::chrono::steady_clock::now();

    while (true) {
        int res = enet_host_service(client, &event, 500);
        if (res > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {
                    std::cout << "--- Received packet (channel " << (int)event.channelID << ", length=" << event.packet->dataLength << ") ---" << std::endl;
                    hexdump(event.packet->data, event.packet->dataLength);
                    // Try to print as text
                    std::string asText(reinterpret_cast<char*>(event.packet->data), event.packet->dataLength);
                    std::cout << "ASCII: " << asText << std::endl;
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Disconnected from server (data=" << event.data << ")" << std::endl;
                    goto cleanup;
                case ENET_EVENT_TYPE_NONE:
                default:
                    break;
            }
        }

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > receiveTimeoutMs) {
            std::cout << "Receive timeout reached, exiting." << std::endl;
            break;
        }
    }

cleanup:
    enet_peer_disconnect(peer, 0);
    // Allow some time for the disconnect to succeed
    while (enet_host_service(client, &event, 3000) > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) enet_packet_destroy(event.packet);
        if (event.type == ENET_EVENT_TYPE_DISCONNECT) break;
    }

    enet_peer_reset(peer);
    enet_host_destroy(client);
    return 0;
}

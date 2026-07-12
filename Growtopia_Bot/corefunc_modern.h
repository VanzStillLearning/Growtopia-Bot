
//    Growtopia Bot
//    Copyright (C) 2018  Growtopia Noobs
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "enet/enet.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <memory>
#include <cstring>

// Platform detection
#ifdef _WIN32
    #define PLATFORM_WINDOWS 1
    #include <conio.h>
    #define PAUSE_CONSOLE() _getch()
#elif __linux__
    #define PLATFORM_LINUX 1
    #define PAUSE_CONSOLE() getchar()
#elif __APPLE__
    #define PLATFORM_MACOS 1
    #define PAUSE_CONSOLE() getchar()
#else
    #define PLATFORM_UNKNOWN 1
    #define PAUSE_CONSOLE() getchar()
#endif

// Modern C++ aliases
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::make_shared;

// Debug macro
#define DEBUG // Comment out for disabling debug features

void dbgPrint(const string& text)
{
#ifdef DEBUG
    std::cout << "[DBG] " << text << std::endl;
#endif
}

/*********  GrowtopiaBot Core Class  ********/
class GrowtopiaBot {
public:
    // Network
    ENetPeer* peer = nullptr;
    ENetHost* client = nullptr;

    // Login state
    int login_user = 0;
    int login_token = 0;

    // World info
    string currentWorld;
    int timeFromWorldEnter = 0;  // in 10ms units
    string gameVersion = "2.98";

    // User data
    int owner = -1;
    string uname;
    string upass;
    string worldName;
    bool publicOwnership = false;
    bool isFollowing = false;
    bool backwardWalk = false;

    // Respawn position
    int respawnX = 0;
    int respawnY = 0;

    // Object data structure
    struct ObjectData {
        int netId = -1;
        int userId = -1;
        string name;
        string country;
        string objectType;  // "avatar" for players
        float x = -1.0f;
        float y = -1.0f;
        bool isGone = false;
        int rectX = 0, rectY = 0;
        int rectWidth = 0, rectHeight = 0;
        bool isMod = false;
        bool isLocal = false;
    };

    vector<ObjectData> objects;

    // Player movement struct
    struct PlayerMoving {
        int netID = 0;
        float x = 0.0f;
        float y = 0.0f;
        int characterState = 0;
        int plantingTree = 0;
        float XSpeed = 0.0f;
        float YSpeed = 0.0f;
        int punchX = 0;
        int punchY = 0;
    };

    // World struct
    struct WorldStruct {
        int XSize = 0;
        int YSize = 0;
        int tileCount = 0;
        string name;
        unique_ptr<int16_t[]> foreground;
        unique_ptr<int16_t[]> background;
    };

    unique_ptr<WorldStruct> world;

    // Constructors
    GrowtopiaBot() = default;
    
    GrowtopiaBot(const string& username, const string& password)
        : uname(username), upass(password) {}

    // Virtual destructor for proper cleanup
    virtual ~GrowtopiaBot() = default;

    // User functions
    virtual void userRender() {}
    virtual void userRender2() {}
    virtual void userLoop() {}
    virtual void userInit() {}
    virtual void onLoginRequested() {}
    virtual void eventLoop() {}

    // Packet handlers
    virtual void packet_type3(const string& text) {}
    virtual void packet_type6(const string& text) {}
    virtual void packet_unknown(ENetPacket* packet) {}

    // Event callbacks
    virtual void OnSendToServer(const string& address, int port, int userId, int token) {}
    virtual void OnConsoleMessage(const string& message) {}
    virtual void OnPlayPositioned(const string& sound) {}
    virtual void OnSetFreezeState(int state) {}
    virtual void OnRemove(const string& data) {}
    virtual void OnSpawn(const string& data) {}
    virtual void OnAction(const string& command) {}
    virtual void SetHasGrowID(int state, const string& name, const string& password) {}
    virtual void SetHasAccountSecured(int state) {}
    virtual void OnTalkBubble(int netID, const string& bubbleText, int type) {}
    virtual void SetRespawnPos(int respawnPos) {}
    virtual void OnEmoticonDataChanged(int val1, const string& emoticons) {}
    virtual void OnSetPos(float x, float y) {}
    virtual void OnAddNotification(const string& image, const string& message, 
                                   const string& audio, int val1) {}
    virtual void AtApplyTileDamage(int x, int y, int damage) {}
    virtual void AtApplyLock(int x, int y, int itemId) {}
    virtual void AtPlayerMoving(const PlayerMoving* data) {}
    virtual void AtAvatarSetIconState(int netID, int state) {}
    virtual void WhenConnected() {}
    virtual void WhenDisconnected() {}
    virtual void respawn() {}
};

/*********  ENet Network Core  ********/

// Send packet with type and data
inline void SendPacket(int type, const string& data, ENetPeer* peer)
{
    if (!peer) return;

    size_t totalSize = data.length() + 5;
    ENetPacket* packet = enet_packet_create(nullptr, totalSize, ENET_PACKET_FLAG_RELIABLE);
    
    if (!packet) return;

    std::memcpy(packet->data, &type, 4);
    std::memcpy(packet->data + 4, data.c_str(), data.length());
    
    enet_peer_send(peer, 0, packet);
}

// Send raw packet with custom data
inline void SendPacketRaw(int type, const void* packetData, size_t packetDataSize, 
                          void* extraData, ENetPeer* peer, int packetFlag)
{
    if (!peer) return;

    ENetPacket* packet = nullptr;
    
    if (type == 4 && packetData) {
        // Special handling for type 4 packets
        uint8_t* p = static_cast<uint8_t*>(const_cast<void*>(packetData));
        if (p[12] & 8) {
            uint32_t extraSize = *reinterpret_cast<uint32_t*>(p + 52);
            size_t totalSize = packetDataSize + extraSize + 5;
            packet = enet_packet_create(nullptr, totalSize, packetFlag);
            
            std::memcpy(packet->data, &type, 4);
            std::memcpy(packet->data + 4, packetData, packetDataSize);
            if (extraData) {
                std::memcpy(packet->data + packetDataSize + 4, extraData, extraSize);
            }
        } else {
            size_t totalSize = packetDataSize + 5;
            packet = enet_packet_create(nullptr, totalSize, packetFlag);
            
            std::memcpy(packet->data, &type, 4);
            std::memcpy(packet->data + 4, packetData, packetDataSize);
        }
    } else {
        size_t totalSize = packetDataSize + 5;
        packet = enet_packet_create(nullptr, totalSize, packetFlag);
        
        std::memcpy(packet->data, &type, 4);
        std::memcpy(packet->data + 4, packetData, packetDataSize);
    }

    if (packet) {
        enet_peer_send(peer, 0, packet);
    }
}

// Connect to server
inline bool connectClient(const string& hostName, int port, ENetHost*& client, ENetPeer*& peer)
{
    client = enet_host_create(nullptr, 1, 2, 0, 0);
    
    if (!client) {
        std::cerr << "Error: Failed to create ENet client host\n";
        return false;
    }

    ENetAddress address;
    client->checksum = enet_crc32;
    enet_host_compress_with_range_coder(client);
    
    if (enet_address_set_host(&address, hostName.c_str()) < 0) {
        std::cerr << "Error: Failed to resolve host\n";
        return false;
    }
    
    address.port = port;
    
    peer = enet_host_connect(client, &address, 2, 0);
    
    if (!peer) {
        std::cerr << "Error: No available peers for connection\n";
        return false;
    }

    enet_host_flush(client);
    return true;
}

/*********  Packet utility functions  ********/

inline int GetMessageTypeFromPacket(ENetPacket* packet)
{
    if (!packet || packet->dataLength <= 3) {
        dbgPrint("Bad packet length");
        return 0;
    }
    return *packet->data;
}

inline char* GetTextPointerFromPacket(ENetPacket* packet)
{
    if (!packet || packet->dataLength < 5) return nullptr;
    
    *(packet->data + packet->dataLength - 1) = 0;
    return reinterpret_cast<char*>(packet->data + 4);
}

#endif // COREFUNC_MODERN_H

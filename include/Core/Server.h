#pragma once

#include "vector"
#include "thread"
#include "queue"
#include "fstream"
#include "map"

#ifdef SE_LINUX
#include "unistd.h"
#endif

#include "Networking/Config.h"
#include "Networking/INetwork.h"
#include "Networking/GetNetCommandEvent.h"

#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Server.Generated.h"

#include "Event/localevent.h"

namespace SteelEngine {

    SE_CLASS(SteelEngine::SE_SERVER_INFO = SteelEngine::ServerInfo(1024, 54000))
    class Server : public Interface::INetwork
    {
        GENERATED_BODY
    public:
        typedef std::map<SOCKET, std::queue<NetworkCommands::MessageData>> SocketMap;

    private:
        fd_set m_Master;
        SOCKET m_ListeningSocket;
        ServerInfo m_ServerInfo;
        SocketMap m_Commands;
        std::vector<NetworkCommands::INetworkCommand*> m_CommandTypes;
        std::string m_Header;
        char* m_Buffer;

    public:
        Server();
        ~Server();

        void Update() override;

        SE_METHOD()
        void Start();

        SE_METHOD()
        int Send(SOCKET sock, const char* buffer, Type::uint32 size) override;

        SE_METHOD()
        int Receive(SOCKET sock, char* buffer, Type::uint32 size) override;

        SE_METHOD()
        std::queue<NetworkCommands::MessageData>* GetCommands(SOCKET sock);

        void operator()(NetworkCommands::INetworkCommand* event);
    };
    
}
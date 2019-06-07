#include "INetworkCommand.Generated.h"
#include "../include\Networking\INetworkCommand.h"
#include "RuntimeCompiler/IRuntimeObject.h"
#include "RuntimeReflection/Reflection.h"

REGISTER_REFLECTION
{
SteelEngine::Reflection::Register<SteelEngine::NetworkCommands::INetworkCommand>("INetworkCommand")
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SteelEngine::NetworkCommands::INetworkCommand))
)
.Inheritance("Interface::IRuntimeObject")
.Property("m_Flow", &SteelEngine::NetworkCommands::INetworkCommand::m_Flow)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NET_VALUE, true)
)
.Property("m_Header", &SteelEngine::NetworkCommands::INetworkCommand::m_Header)
(
SteelEngine::Reflection::MetaData(SteelEngine::ReflectionAttribute::SE_NET_VALUE, true)
)
.Property("m_Commands", &SteelEngine::NetworkCommands::INetworkCommand::m_Commands)
(
)
;
}

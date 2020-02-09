#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "RuntimeReflection/Reflection.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/RuntimeReflection/Reflection.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Reflection>("Reflection",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::NO_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Reflection))
)
.Inheritance<StaticHelper>("StaticHelper")
.Enum<Reflection::ReflectionAttribute>("ReflectionAttribute")
.Values({
SteelEngine::Reflection::EnumElement("RUNTIME_SERIALIZE", Reflection::ReflectionAttribute::RUNTIME_SERIALIZE),
SteelEngine::Reflection::EnumElement("UPDATE", Reflection::ReflectionAttribute::UPDATE),
SteelEngine::Reflection::EnumElement("REFLECTION_MODULE", Reflection::ReflectionAttribute::REFLECTION_MODULE),
SteelEngine::Reflection::EnumElement("NO_SERIALIZE", Reflection::ReflectionAttribute::NO_SERIALIZE),
SteelEngine::Reflection::EnumElement("NET_VALUE", Reflection::ReflectionAttribute::NET_VALUE),
SteelEngine::Reflection::EnumElement("NETWORK_COMMAND", Reflection::ReflectionAttribute::NETWORK_COMMAND),
SteelEngine::Reflection::EnumElement("EDITOR", Reflection::ReflectionAttribute::EDITOR),
SteelEngine::Reflection::EnumElement("EDITOR_NAME", Reflection::ReflectionAttribute::EDITOR_NAME),
SteelEngine::Reflection::EnumElement("PROJECTS_PATH", Reflection::ReflectionAttribute::PROJECTS_PATH),
SteelEngine::Reflection::EnumElement("VIRTUAL_PROJECT", Reflection::ReflectionAttribute::VIRTUAL_PROJECT),
SteelEngine::Reflection::EnumElement("GAME_SCRIPT", Reflection::ReflectionAttribute::GAME_SCRIPT),
SteelEngine::Reflection::EnumElement("ENGINE_START_TYPE", Reflection::ReflectionAttribute::ENGINE_START_TYPE),
SteelEngine::Reflection::EnumElement("GENERATE_CAST_FUNCTIONS", Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS),
SteelEngine::Reflection::EnumElement("DO_NOT_GENERATE_CAST_FUNCTIONS", Reflection::ReflectionAttribute::DO_NOT_GENERATE_CAST_FUNCTIONS),
SteelEngine::Reflection::EnumElement("SYSTEMS_INFORMATION_TRACKER", Reflection::ReflectionAttribute::SYSTEMS_INFORMATION_TRACKER),
SteelEngine::Reflection::EnumElement("NO_RECOMPILE", Reflection::ReflectionAttribute::NO_RECOMPILE),
SteelEngine::Reflection::EnumElement("INHERITANCE_MODULE", Reflection::ReflectionAttribute::INHERITANCE_MODULE),
SteelEngine::Reflection::EnumElement("NAIVE_EVENT", Reflection::ReflectionAttribute::NAIVE_EVENT)
})
;
}
}

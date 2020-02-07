#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ConsoleSystem/Commands/TestCommand.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ConsoleSystem/Commands/TestCommand.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<TestCommand>("TestCommand",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CONSOLE_COMMAND, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(TestCommand))
)
.Constructor<>()
.Inheritance<ICommand>("ICommand")
;
}
void TestCommand::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(TestCommand)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}

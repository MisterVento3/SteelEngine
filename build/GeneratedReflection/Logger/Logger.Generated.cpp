#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Logger/Logger.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Logger/Logger.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Logger>("Logger",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Logger))
)
.Constructor<const char*>()
.Inheritance<ILogger>("ILogger")
.Method("Init", &Logger::Init)
.Method<void, const std::string&, int, Type::uint32, const std::filesystem::path&, va_list>("Log", &Logger::Log)
.Method<void, const std::string&, int, va_list>("Log", &Logger::Log)
;
}
}

#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "PythonCore/Script.h"
#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/PythonCore/Script.Generated.h"

namespace SteelEngine {
namespace Script {
namespace Python {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Script>("Script",{
"SteelEngine",
"Script",
"Python"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(Script))
)
.Constructor<>()
.Inheritance<IScript>("IScript")
;
}
}
}
}

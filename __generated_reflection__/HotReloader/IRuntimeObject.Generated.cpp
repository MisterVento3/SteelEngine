#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "HotReloader/IRuntimeObject.h"
#include "HotReloader/IRuntimeObject.Generated.h"

namespace SteelEngine {
namespace HotReloader {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<IRuntimeObject>("IRuntimeObject",{
"SteelEngine",
"HotReloader"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(IRuntimeObject))
)
;
}
}
}

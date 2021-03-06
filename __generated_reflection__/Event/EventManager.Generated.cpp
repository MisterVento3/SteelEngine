#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "Event/EventManager.h"
#include "Event/EventManager.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<EventManager>("EventManager",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(EventManager))
)
.RegisterConstructor<>()
.RegisterInheritance<IEventManager>("IEventManager")
;
}
}

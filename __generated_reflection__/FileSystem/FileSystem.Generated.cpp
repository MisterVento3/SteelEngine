#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "FileSystem/FileSystem.h"
#include "D:\Projects\C++\SteelEngine\__generated_reflection__/FileSystem/FileSystem.Generated.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<FileSystem>("FileSystem",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData("sizeof", sizeof(FileSystem))
)
.RegisterConstructor<>()
.RegisterInheritance<IFileSystem>("IFileSystem")
;
}
}
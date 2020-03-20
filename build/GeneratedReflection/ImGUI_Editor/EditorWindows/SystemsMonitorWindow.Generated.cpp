#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"
#include "ImGUI_Editor/EditorWindows/SystemsMonitorWindow.h"
#include "D:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/SystemsMonitorWindow.Generated.h"

namespace SteelEngine {
namespace Editor {
namespace ImGUI {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<SystemsMonitorWindow>("SystemsMonitorWindow",{
"SteelEngine",
"Editor",
"ImGUI"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE , SteelEngine::Editor::SceneType::START_MENU_SCENE | SteelEngine::Editor::SceneType::EDITOR_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData(Reflection::ReflectionAttribute::HOT_RELOAD, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(SystemsMonitorWindow))
)
.Constructor<>()
.Inheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.Inheritance<HotReloader::IRuntimeObject>("HotReloader::IRuntimeObject")
.Method("Cast_UserInterface", &SystemsMonitorWindow::Cast_UserInterface)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Cast_IRuntimeObject", &SystemsMonitorWindow::Cast_IRuntimeObject)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::CAST_FUNCTION, true)
)
.Method("Serialize", &SystemsMonitorWindow::Serialize)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::SERIALIZE_FUNCTION, true)
)
;
}
void SystemsMonitorWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(SystemsMonitorWindow::m_CurrentItem)
SERIALIZE(SystemsMonitorWindow::m_CurrentReflectionItem)
SERIALIZE(SystemsMonitorWindow::reflectionRes)
SERIALIZE(SystemsMonitorWindow::m_Info)
SERIALIZE(SystemsMonitorWindow::m_Database)
UserInterface::Serialize(serializer);
}
#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(SystemsMonitorWindow)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}
}
}

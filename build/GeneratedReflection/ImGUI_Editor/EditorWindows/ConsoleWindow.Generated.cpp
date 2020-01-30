#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/ImGUI_Editor/EditorWindows/ConsoleWindow.Generated.h"
#include "ImGUI_Editor/EditorWindows/ConsoleWindow.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<ConsoleWindow>("ConsoleWindow",{
"SteelEngine"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::SCENE_TYPE, SteelEngine::Editor::SceneType::EDITOR_SCENE|SteelEngine::Editor::SceneType::START_MENU_SCENE),
SteelEngine::Reflection::MetaData(SteelEngine::Editor::ReflectionAttributes::EDITOR_WINDOW, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData(SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::SEPARATE_WINDOW, true),
SteelEngine::Reflection::MetaData(SteelEngine::EditorComponents::ImGUI::UserInterface::Attributes::FLAGS, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(ConsoleWindow))
)
.Inheritance<EditorComponents::ImGUI::UserInterface>("EditorComponents::ImGUI::UserInterface")
.Inheritance<LogDispatcher>("LogDispatcher")
.Inheritance<EventObserver>("EventObserver")
.Constructor<>()
.Method("Cast_UserInterface", &ConsoleWindow::Cast_UserInterface)
.Method("Cast_LogDispatcher", &ConsoleWindow::Cast_LogDispatcher)
.Method("Cast_EventObserver", &ConsoleWindow::Cast_EventObserver)
;
}

void ConsoleWindow::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(ConsoleWindow::m_Logs)
SERIALIZE(ConsoleWindow::m_ScrollDown)
SERIALIZE(ConsoleWindow::m_CommandBuffer)
SERIALIZE(ConsoleWindow::m_Keys)
SERIALIZE(ConsoleWindow::m_ConsoleSystem)
UserInterface::Serialize(serializer);
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(ConsoleWindow)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR_()
};

return result;
}
#endif
}

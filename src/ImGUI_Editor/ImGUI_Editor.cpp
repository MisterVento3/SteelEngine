#include "ImGUI_Editor/ImGUI_Editor.h"

#include "imgui/imgui_internal.h"

#include "algorithm"

#include "RuntimeReflection/Reflection.h"

#include "Core/Core.h"

#undef min
#undef max

#define SE_EDITOR_NOT_IMPLEMENTED_ADDNOTATION \
    if(ImGui::IsItemHovered()) \
    { \
        ImGui::BeginTooltip(); \
        { \
            ImGui::Text("Not implemented yet!"); \
        } \
        ImGui::EndTooltip(); \
    }

namespace SteelEngine { namespace Editor { namespace ImGUI {

    void ImGUI_Editor::LoadProject()
    {

    }

    void ImGUI_Editor::CreateNewProject()
    {

    }

    ImGUI_Editor::ImGUI_Editor()
    {
        m_CurrentScene = SceneType::START_MENU_SCENE;
    }

    ImGUI_Editor::~ImGUI_Editor()
    {

    }

    Result ImGUI_Editor::Init(Graphics::IRenderer* renderer, IContext* context)
    {
        m_API_Context = context;
        m_Context = (ImGuiContext*)Reflection::GetType("SteelEngine::VulkanContext")->Invoke("GetContext", context).Convert<void*>();

        ImGui::SetCurrentContext(m_Context);

        IReflectionData** types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            IReflectionData* type = types[i];
            std::vector<IReflectionInheritance*> inhs = type->GetInheritances();

            for(Type::uint32 i = 0; i < inhs.size(); i++)
            {
                if(inhs[i]->GetTypeID() == typeid(EditorComponents::ImGUI::UserInterface).hash_code() && type->GetMetaData(ReflectionAttributes::EDITOR_WINDOW)->Convert<bool>())
                {
                    EditorComponents::ImGUI::UserInterface** window = (EditorComponents::ImGUI::UserInterface**)&type->Create()->m_Object;

                    strcpy((*window)->m_Title, type->GetTypeName().c_str());
                    (*window)->m_Context = m_Context;
                    (*window)->m_Editor = (ImGUI_Editor**)&m_Object;

                    if(type->GetMetaData(ReflectionAttributes::EDITOR_WINDOW)->Convert<bool>())
                    {
                        if(type->GetMetaData(ReflectionAttributes::SCENE_TYPE)->Convert<SceneType>() & SceneType::EDITOR_SCENE)
                        {
                            m_MainEditorWindows.push_back(window);
                        }

                        if(type->GetMetaData(ReflectionAttributes::SCENE_TYPE)->Convert<SceneType>() & SceneType::START_MENU_SCENE)
                        {
                            m_StartMenuWindows.push_back(window);
                        }
                    }

                    (*window)->Init();
                }
            }
        }

        IEditor::Init(*m_VirtualProjectVisualizer, context);

        Event::GlobalEvent::Add<ChangeSceneEvent>(this);

        return SE_TRUE;
    }

    void ImGUI_Editor::Draw()
    {
        ImGui::NewFrame();

        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Project"))
            {
                if(ImGui::MenuItem("New"))
                {

                }

                SE_EDITOR_NOT_IMPLEMENTED_ADDNOTATION

                if(ImGui::MenuItem("Load"))
                {

                }

                SE_EDITOR_NOT_IMPLEMENTED_ADDNOTATION

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if(!GImGui || !GImGui->FrameScopeActive)
        {
            printf("Skipping frame!\n");

            return;
        }

        Render(*m_VirtualProjectVisualizer);

        if(m_CurrentScene == SceneType::START_MENU_SCENE)
        {
            for(Type::uint32 i = 0; i < m_StartMenuWindows.size(); i++)
            {
                HotReloader::IRuntimeObject* uiObject = (*m_StartMenuWindows[i]);
                EditorComponents::ImGUI::UserInterface* ui = Reflection::GetType(uiObject)->Invoke("Cast_UserInterface", uiObject).Convert<EditorComponents::ImGUI::UserInterface*>();

                if(Reflection::GetType(uiObject)->GetMetaData(EditorComponents::ImGUI::UserInterface::SEPARATE_WINDOW)->Convert<bool>())
                {
                    ImGui::Begin(ui->m_Title);
                    {
                        ui->Draw();
                    }
                    ImGui::End();
                }
                else
                {
                    ui->Draw();
                }
            }
        }
        else if(m_CurrentScene == SceneType::EDITOR_SCENE)
        {
            for(Type::uint32 i = 0; i < m_UIs.size(); i++)
            {
                HotReloader::IRuntimeObject* uiObject = (*m_UIs[i]);
                EditorComponents::ImGUI::UserInterface* ui = Reflection::GetType(uiObject)->Invoke("Cast_UserInterface", uiObject).Convert<EditorComponents::ImGUI::UserInterface*>();

                if(Reflection::GetType(uiObject)->GetMetaData(EditorComponents::ImGUI::UserInterface::SEPARATE_WINDOW)->Convert<bool>())
                {
                    ImGui::Begin(ui->m_Title);
                    {
                        ui->Draw();
                    }
                    ImGui::End();
                }
                else
                {
                    ui->Draw();
                }
            }

            for(Type::uint32 i = 0; i < m_MainEditorWindows.size(); i++)
            {
                HotReloader::IRuntimeObject* uiObject = (*m_MainEditorWindows[i]);
                EditorComponents::ImGUI::UserInterface* ui = Reflection::GetType(uiObject)->Invoke("Cast_UserInterface", uiObject).Convert<EditorComponents::ImGUI::UserInterface*>();

                if(Reflection::GetType(uiObject)->GetMetaData(EditorComponents::ImGUI::UserInterface::SEPARATE_WINDOW)->Convert<bool>())
                {
                    ImGui::Begin(ui->m_Title);
                    {
                        ui->Draw();

                        const std::vector<IReflectionData::PropertyInfo> a = Reflection::GetType(uiObject)->GetProperties();

                        for(Type::uint32 i = 0; i < a.size(); i++)
                        {
                            IReflectionData::PropertyInfo info = a[i];
                            Variant prop = Reflection::GetType(uiObject)->GetProperty(info.m_Name, uiObject);

                            if(prop.GetType() == m_FloatTypeID)
                            {
                                Variant* meta = info.m_Property->GetMetaData(ReflectionAttributes::RANGE);

                                if(meta->IsValid())
                                {
                                    Range range = meta->Convert<Range>();

                                    ImGui::SliderFloat(info.m_Name.c_str(), (float*)prop.GetAddress(), range.m_Min, range.m_Max);
                                }
                            }
                            else if(prop.GetType() == m_IntTypeID)
                            {
                                Variant* meta = info.m_Property->GetMetaData(ReflectionAttributes::RANGE);

                                if(meta->IsValid())
                                {
                                    Range range = meta->Convert<Range>();

                                    ImGui::SliderInt(info.m_Name.c_str(), (int*)prop.GetAddress(), range.m_Min, range.m_Max);
                                }
                            }
                        }
                    }
                    ImGui::End();
                }
                else
                {
                    ui->Draw();
                }
            }
        }

        ImGui::Render();
    }

    void ImGUI_Editor::ProcessEvents(void* event)
    {
        m_API_Context->ProcessEvent(event);
    }

    void ImGUI_Editor::OnRecompile(HotReloader::IRuntimeObject* oldObject)
    {
        ImGui::SetCurrentContext(m_Context);
    }

    void ImGUI_Editor::operator()(const ChangeSceneEvent& event)
    {
        m_CurrentScene = event.m_SceneType;

        IReflectionData** types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            IReflectionData* type = types[i];

            bool found = false;

            for(EditorComponents::ImGUI::UserInterface** wnd : m_MainEditorWindows)
            {
                if((*wnd)->m_TypeID == type->GetTypeID())
                {
                    found = true;
                }
            }

            for(EditorComponents::ImGUI::UserInterface** wnd : m_StartMenuWindows)
            {
                if((*wnd)->m_TypeID == type->GetTypeID())
                {
                    found = true;
                }
            }

            if(!found)
            {
                std::vector<IReflectionInheritance*> inhs = type->GetInheritances();

                for(Type::uint32 i = 0; i < inhs.size(); i++)
                {
                    if(inhs[i]->GetTypeID() == Reflection::GetType(SE_GET_TYPE_NAME(SteelEngine::EditorComponents::ImGUI::UserInterface))->GetTypeID())
                    {
                        if(type->GetMetaData(ReflectionAttribute::NETWORK_COMMAND)->Convert<bool>())
                        {
                            Network::INetworkManager** manager =
                                Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::NETWORK_MANAGER)->Convert<Network::INetworkManager**>();
                            std::vector<Network::INetworkCommand*> commands = (*manager)->GetCommands();

                            for(Network::INetworkCommand* cmd : commands)
                            {
                                if(type->GetTypeID() == cmd->m_TypeID)
                                {
                                    EditorComponents::ImGUI::UserInterface* ui = type->Invoke("Cast_UserInterface", cmd->m_Object).Convert<EditorComponents::ImGUI::UserInterface*>();

                                    strcpy(ui->m_Title, type->GetTypeName().c_str());

                                    ui->m_Context = m_Context;

                                    ui->Init();

                                    m_UIs.push_back(&(cmd->m_Object));
                                }
                            }
                        }
                    }
                }
            }

            // if(!found && type->GetMetaData(Utils::Editor::UserInterface::SEPARATE_WINDOW)->Convert<bool>())
            // {
            //     if(type->GetMetaData(ReflectionAttribute::NETWORK_COMMAND)->Convert<bool>())
            //     {
            //         Network::INetworkManager** manager =
            //             Reflection::GetType("SteelEngine::Core")->GetMetaData(Core::GlobalSystems::NETWORK_MANAGER)->Convert<Network::INetworkManager**>();

            //         std::vector<Network::INetworkCommand*> commands = (*manager)->GetCommands();

            //         for(Network::INetworkCommand* cmd : commands)
            //         {
            //             if(type->GetTypeID() == cmd->m_TypeID)
            //             {
            //                 Utils::Editor::UserInterface* ui = type->Invoke("Cast_UserInterface", cmd->m_Object).Convert<Utils::Editor::UserInterface*>();

            //                 strcpy(ui->m_Title, type->GetTypeName().c_str());

            //                 ui->m_Context = m_Context;

            //                 ui->Init();

            //                 m_OtherWindows.push_back(ui);
            //             }
            //         }
            //     }
            //     else
            //     {
            //         Window** wnd = (Window**)&type->Create()->m_Object;

            //         m_Windows.push_back(wnd);

            //         (*wnd)->m_Title = type->GetTypeName();
            //         (*wnd)->m_Context = m_Context;

            //         (*wnd)->Init();
            //     }

            //     break;
            // }
        }
    }

}}}
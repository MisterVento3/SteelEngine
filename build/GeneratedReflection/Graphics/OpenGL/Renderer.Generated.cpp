#include "d:\Projects\C++\SteelEngine/build/GeneratedReflection/Graphics/OpenGL/Renderer.Generated.h"
#include "Graphics/OpenGL/Renderer.h"
#include "HotReloader/IRuntimeObject.h"
#include "RuntimeReflection/ReflectionRecorder.h"

namespace SteelEngine {
namespace Graphics {
namespace OpenGL {
REGISTER_REFLECTION
{
SteelEngine::ReflectionRecorder::Register<Renderer>("Renderer",{
"SteelEngine",
"Graphics",
"OpenGL"
}
)
(
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::RUNTIME_SERIALIZE, true),
SteelEngine::Reflection::MetaData(SteelEngine::Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS, true),
SteelEngine::Reflection::MetaData("sizeof", sizeof(Renderer))
)
.Inheritance<IRenderer>("IRenderer")
.Inheritance<EventObserver>("EventObserver")
.Constructor<IWindow*>()
.Method("Cast_IRenderer", &Renderer::Cast_IRenderer)
.Method("Cast_EventObserver", &Renderer::Cast_EventObserver)
;
}

void Renderer::Serialize(SteelEngine::HotReloader::ISerializer* serializer)
{
SERIALIZE(Renderer::m_Window)
SERIALIZE(Renderer::m_Camera)
SERIALIZE(Renderer::m_QuadShader)
SERIALIZE(Renderer::m_QuadMesh)
SERIALIZE(Renderer::m_Lights)
SERIALIZE(Renderer::m_Texture)
SERIALIZE(Renderer::m_NormalMapTexture)
SERIALIZE(Renderer::m_DispMapTexture)
SERIALIZE(Renderer::m_G_Shader)
SERIALIZE(Renderer::m_PositionTexture)
SERIALIZE(Renderer::m_NormalTexture)
SERIALIZE(Renderer::m_AlbedoTexture)
SERIALIZE(Renderer::m_ShadowTexture)
SERIALIZE(Renderer::m_G_Buffer)
SERIALIZE(Renderer::m_ShadowShader)
SERIALIZE(Renderer::m_ShadowCamera)
SERIALIZE(Renderer::m_SpotLight)
SERIALIZE(Renderer::m_SpotRotation)
SERIALIZE(Renderer::m_DirectionalLight)
SERIALIZE(Renderer::m_DirectionalLightTransform)
SERIALIZE(Renderer::m_Keys)
SERIALIZE(Renderer::m_RotateCamera)
SERIALIZE(Renderer::m_Controlls)
}

#ifdef RUNTIME_COMPILE
extern "C" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)
{
DECLARE_TYPE_INFO(Renderer)
{
FIND_THE_RIGHT_OBJECT

COMPARE_CONSTRUCTOR(IWindow*)
};

return result;
}
#endif
}
}
}

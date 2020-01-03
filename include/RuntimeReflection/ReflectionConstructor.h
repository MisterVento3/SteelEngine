#pragma once

#include "RuntimeReflection/NoTupleInvoke.h"

#include "Utils/Reflection.h"

#include "HotReloader/IRuntimeObject.h"

#include "ModuleManager/ModuleManager.h"

namespace SteelEngine {

	template <typename... Args>
	struct ReflectionConstructor : public NoTupleReflectionConstructor
	{
		typedef void*(*Func)(Args...);

		Func m_Function;
		MetaDataInfoVector m_MetaData;

		ReflectionConstructor(Func func) :
			m_Function(func)
		{

		}

		MetaDataInfoVector* GetMetaDataInfoVector() override
		{
			return &m_MetaData;
		}

		HotReloader::IRuntimeObject* Invoke(ITuple* args) override
		{
			static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

			Tuple<Args...>* tuple = 0;

			if(args->GetTuple())
			{
				tuple = (Tuple<Args...>*)args->GetTuple();
			}
			else
			{
				tuple = (Tuple<Args...>*)args;
			}

			HotReloader::IRuntimeObject* object = (HotReloader::IRuntimeObject*)apply(m_Function, tuple->m_Args);

			object->m_Object = object;
			object->m_ConstructorID = m_ConstructorID;
			object->m_ObjectID = db->GetNextPerObjectID();
			object->m_TypeID = m_TypeID;

			return object;
		}
	};

}
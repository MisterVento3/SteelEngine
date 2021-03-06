#include "HotReloader/RuntimeReloader.h"

#include "Module/ModuleExport.h"

#include "ModuleManager/ModuleManager.h"

#include "Utils/TimeCheck.h"
#include "Utils/Utils.h"

#include "RuntimeReflection/Reflection.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

#include "Core/Core.h"
#include "Core/GetCompileConfigEvent.h"

#include "fstream"
#include "sstream"

#include "FileSystem/FileSystem.h"

#include "Profiler/ScopeTimer.h"

#include "HotReloader/ReloadableInheritanceTrackKeeper.h"

#ifdef SE_WINDOWS
#include "crtdefs.h"
#include "process.h"
#endif

namespace SteelEngine { namespace HotReloader {

	typedef TypeInfo*(*GetPerModule)(RuntimeDatabase::ConstructedObjectsVector*);
	typedef size_t(*GetTypeID_Callback)();

    const std::string RuntimeReloader::mcs_CompletionToken = "_COMPLETION_TOKEN_";

	void RuntimeReloader::GenerateModuleName()
	{
#ifdef SE_WINDOWS
		m_ModuleName = std::string(tmpnam(0));

		std::vector<std::string> splitted = split(m_ModuleName, '\\');

		m_ModuleName = splitted[splitted.size() - 1];
#else
		m_ModuleName = "build" + std::string(tmpnam(0)) + ".so";
#endif
	}

	void RuntimeReloader::Test(const std::filesystem::path& file, FileWatcher::FileStatus status)
	{
		
	}

    RuntimeReloader::RuntimeReloader()
    {
		m_IsSwapComplete = true;
		m_Paused = false;
    }

    RuntimeReloader::~RuntimeReloader()
    {
		
    }

    Result RuntimeReloader::Initalize()
    {
		SE_INFO("Initializing runtime hot-reloader!");

		GetCompileConfigEvent event;

		Event::GlobalEvent::Broadcast_(&event);

		m_CompileConfig = event.m_Config;

		void* dll;
		ProcessAllocator allocate;
		SteelEngine::Module::load("Subprocess.dll", &dll);
		SteelEngine::Module::get("Subprocess_new", dll, (void**)&allocate);

		m_Process = allocate();

		if(m_Process->Setup() == SE_FALSE)
        {
            return SE_FALSE;
        }

		std::filesystem::path ppp = FileSystem::Get("fileWatcherPath");

		m_Process->WriteInput("cd " + ppp.string() + "/build/Windows\n" + mcs_CompletionToken + "\n");

		m_BinaryLocation = getBinaryLocation();

		printf("WatchDog is watching at: %s\n", ppp.string().c_str());

        m_SourceFileWatcher = new FileWatcher(
			ppp / "src",
			[this, ppp](const std::filesystem::path& file, FileWatcher::FileStatus status)
			{
				if(std::filesystem::is_regular_file(file) && m_Process->IsCompileComplete() && m_IsSwapComplete)
				{
					switch(status)
					{
					case FileWatcher::FileStatus::CREATED:
						// SE_INFO("File created: %s", file.string().c_str());
						break;
					case FileWatcher::FileStatus::DELETED:
						break;
					case FileWatcher::FileStatus::MODIFIED:
					{
						// CHECK_SPEED(
						// 	{
								if(file.extension() == ".cpp")
								{
									std::string dirPath = file.parent_path().string();

									printf("File changed: %s\n", dirPath.c_str());

									replaceAll(dirPath, "/", "\\");

									std::vector<std::string> splitted = split(dirPath, '\\');
									std::string p;
									std::string projectsPath = ppp.string();

									replaceAll(projectsPath, "/", "\\");

									size_t splittedSize = split(projectsPath, '\\').size();

									for(Type::uint32 i = splittedSize + 1; i < splitted.size(); i++)
									{
										p.append(splitted[i]); 

										if(i < splitted.size() - 1)
										{
											p.append("\\");
										}
									}

									std::filesystem::path p2 = FileSystem::Get("fileWatcherPath") / "include";

									if(p != "")
									{
										p2 = p2.string() + "\\" + p;
									}

									m_ReflectionGenerator->Load(p2 / file.filename().replace_extension(".h"));
								}

								m_ReflectionGenerator->Parse();
								m_ReflectionGenerator->Generate(FileSystem::Get("fileWatcherPath"), FileSystem::Get("fileWatcherPath") / "__generated_reflection__");
								m_ReflectionGenerator->Clear();
						// 	},
						// 	{
						// 		SE_INFO("Reflection generation time %f ms", delta * 1000.f);
						// 	}
						// );

						Thread* thread = new Thread();

						thread->m_Thread = std::thread([&, thread, file]()
						{
							GenerateModuleName();

							CHECK_SPEED(
								{
									Compile(file.string());
								},
								{
									SE_INFO("Compile time: %f ms", delta * 1000.f);
								}
							)

							if(m_Process->WasError())
							{
								thread->m_Done = true;
								m_Process->SetError(false);

								SE_ERROR("Error while compiling: %s", m_Process->GetErrorMessage().c_str());

								return;
							}

							CHECK_SPEED(
								{
									SwapModule(m_BinaryLocation.string() + "/Runtime/Swap/" + m_ModuleName + ".dll");
								},
								{

								}
							);

#ifdef SE_WINDOWS
						// TODO: Fix memory leak, but we cant just delete it right after broadcasting, but after command finish
							IReflectionData* type = Reflection::GetType("SteelEngine::Network::SwapModuleNetCommandEvent");
							void** ev = type->Create_((m_ModuleName + "\0").c_str());
							InheritanceTrackKeeper* swapper = new ReloadableIneritanceTrackKeeper(type, ev);

							Event::GlobalEvent::Broadcast_(swapper->Get<Network::INetworkCommand>());

							// delete *ev;
							// delete swapper;

							// Reflection::DestroyInstance(ev);
#else
							//SendToOthers(m_ModuleName);
#endif

							thread->m_Done = true;
						});
			
						m_Threads.push_back(thread);
						break;
					}
					}
				}
			}
		);

		m_Compiler = (IRuntimeCompiler*)Reflection::CreateInstance("SteelEngine::RuntimeCompiler", m_Process);

		Event::GlobalEvent::Add<SwapModuleEvent>(this);
		Event::GlobalEvent::Add<StopRecompilingEvent>(this);
		Event::GlobalEvent::Add<StartRecompilingEvent>(this);

		m_Initialized = true;

		m_WatchDogThread = new std::thread([&]()
		{
		// TODO: Change to some state variable
			while(1)
			{
				Sleep(10);

				if(!m_Paused && m_Initialized)
				{
					m_SourceFileWatcher->Update();
				}
			}
		});

        return SE_TRUE;
    }

    void RuntimeReloader::Cleanup()
    {

    }

	void RuntimeReloader::Update()
	{
		for(Type::uint32 i = 0; i < m_Threads.size(); i++)
		{
			Thread* thread = m_Threads[i];

			if(thread && thread->m_Done)
			{
				thread->m_Done = false;

				if(thread->m_Thread.joinable())
				{
					thread->m_Thread.join();
				}

				delete thread;
				thread = 0;

				m_Threads.pop_back();
			}
		}
	}

    void RuntimeReloader::Compile(const std::filesystem::path& file)
    {
		SE_PROFILE_FUNC;

#ifndef SE_WINDOWS
		m_SourceFile = file.string();
		replaceAll(m_SourceFile, "\\", "/");

		std::string generatedFile = file.string();

		replaceAll(generatedFile, "/", "\\");

		std::vector<std::string> splitted = split(generatedFile, '\\');

		generatedFile = splitted[splitted.size() - 1];
		generatedFile = split(generatedFile, '.')[0];
		generatedFile = generatedFile + ".Generated.cpp";
		generatedFile = "__generated_reflection__/" + generatedFile;

		if(!filesystem::exists(generatedFile))
		{
			generatedFile = "";
		}

		std::string sourceFilesToCompile = file.string() + " " + generatedFile;
		std::string objFiles = "";
		std::string libs = "-Lbin -LC:/msys64/mingw64/x86_64-w64-mingw32/lib -lstdc++fs -lModule -lUtils -lws2_32";

		std::string totalCommand = "clang++ -DRUNTIME_COMPILE -std=c++17 -shared -g -Iinclude -Ibuild -o " + m_ModuleName + " " +
			sourceFilesToCompile + " " + objFiles + " " + libs;

		totalCommand += "\n" + cs_CompletionToken + "\n";
#else
		std::string sourceFile = file.string();
		replaceAll(sourceFile, "\\", "/");

		std::string generatedFile = file.string();
		replaceAll(generatedFile, "\\", "/");

		std::vector<std::string> splitted = split(generatedFile, '/');

		generatedFile = FileSystem::Get("fileWatcherPath").string() + "/__generated_reflection__/";
		std::string res = "";
		std::string l = FileSystem::Get("fileWatcherPath").string();

		replaceAll(l, "/", "\\");

		for(Type::uint32 i = split(l, '\\').size() + 1; i < splitted.size() - 1; i++)
		{
			generatedFile.append(splitted[i]).append("/");
			res.append(splitted[i]).append("/");
		}

		generatedFile.append(split(splitted[splitted.size() - 1], '.')[0]).append(".Generated.cpp");

		replaceAll(generatedFile, "\\", "/");

		std::vector<std::string> includeVector = m_CompileConfig["includes"];
		std::vector<std::string> libPathVector = m_CompileConfig["lib_paths"];
		std::vector<std::string> libVector = m_CompileConfig["libs"];
		std::vector<std::string> defineVector = { "UNICODE", "_UNICODE", "RUNTIME_COMPILE", "SE_WINDOWS" };
		std::vector<std::string> flagVector = m_CompileConfig["flags"];

		std::string buildDir = m_BinaryLocation.string() + "/Runtime/Swap/";
		std::string enginePath = getBinaryLocation().parent_path().string();
		std::string sourceFiles = "";

		flagVector.push_back("/Fe" + buildDir + m_ModuleName + ".dll");
		flagVector.push_back("/Fd" + buildDir + m_ModuleName + ".pdb");
		flagVector.push_back("/nologo");
		flagVector.push_back("/FC");

		Utils::json dllOptions = m_CompileConfig["dll"];

		for(Utils::json::iterator it = dllOptions.begin(); it != dllOptions.end(); ++it)
		{
			flagVector.push_back((*it));
		}

		for(std::string& file : includeVector)
		{
			replaceAll(file, "$CWD$", enginePath);
		}

		for(std::string& file : libPathVector)
		{
			replaceAll(file, "$CWD$", enginePath);
		}

		includeVector.push_back(FileSystem::Get("fileWatcherPath").string() + "/include");
		includeVector.push_back(FileSystem::Get("fileWatcherPath").string() + "/__generated_reflection__");

		m_ObjFilesToDelete.push_back(FileSystem::Get("fileWatcherPath").string() + "/build/Windows/" + split(splitted[splitted.size() - 1], '.')[0] + ".obj ");
		m_ObjFilesToDelete.push_back(FileSystem::Get("fileWatcherPath").string() + "/build/Windows/" + split(splitted[splitted.size() - 1], '.')[0] + ".Generated.obj");

		Utils::json modules = Utils::loadJsonFile(getBinaryLocation() / "config.json")["modules"];

		for(Utils::json::iterator it = modules.begin(); it != modules.end(); ++it)
		{
			Utils::json module = (*it);

			if(module["type"] == "lib")
			{
				std::string name = module["name"];

				libVector.push_back(name + ".lib");
			}
		}

		if(res != "")
		{
			res = split(res, '/')[0];
		}

		res = FileSystem::Get("fileWatcherPath").string() + "/build/Windows/" + res;

		if(!std::filesystem::is_directory(res))
		{
			std::filesystem::create_directory(res);
		}
		else
		{
			for(auto& p : std::filesystem::directory_iterator(res))
			{
				if(p.path().extension() == ".obj")
				{
					std::string pp = split(p.path().filename().string(), '.')[0];
					std::string ppp = split(splitted[splitted.size() - 1], '.')[0];

					if(pp != ppp)
					{
						sourceFiles += p.path().string() + " ";

						std::string gene = FileSystem::Get("fileWatcherPath").string() + "/build/Windows/" + pp + ".Generated.obj";

						if(std::filesystem::exists(gene))
						{
							sourceFiles += gene + " ";
						}
					}				
				}
			}
		}

		sourceFiles = sourceFile + " " + generatedFile;

		m_Process->WriteInput("cd " + res + "\n" + mcs_CompletionToken + "\n");

		while(!m_Process->IsCompileComplete())
		{

		}

		m_Compiler->Compile(sourceFiles, flagVector, defineVector, includeVector, libPathVector, libVector);
		m_Compiler->WaitUntilComplete();
#endif
    }

    void RuntimeReloader::SwapModule(const std::string& moduleName)
    {
		SE_INFO("Swapping %s module", moduleName.c_str());
		SE_PROFILE_FUNC;

		std::string moduleName_ = moduleName;
		replaceAll(moduleName_, "/", "\\");

		static RuntimeDatabase* db = (RuntimeDatabase*)ModuleManager::GetModule("RuntimeDatabase");

		void* hotReloadedModule;
		GetPerModule getPerModule;

		try
		{
			Module::load(moduleName.c_str(), &hotReloadedModule);
			Module::get("allocateRuntimeObject", hotReloadedModule, (void**)&getPerModule);

			HotModule hot;

			hot.m_Module = 		hotReloadedModule;
			hot.m_ModuleName = 	moduleName;

			m_LoadedHotModules.push_back(hot);
		}
		catch(const std::exception& e)
		{
			SE_ERROR(e.what());
		}

		if(!getPerModule)
		{
			SE_ERROR("GetPerModule in null!");

			for(std::string f : m_ObjFilesToDelete)
			{
				std::filesystem::remove(f);
			}

			m_ObjFilesToDelete.clear();

			return;
		}

		TypeInfo* info = getPerModule(db->m_HotReloaderDatabase->m_Objects);
		size_t size = db->m_HotReloaderDatabase->m_Objects->Size();
		HotReloader::IRuntimeObject* old = 0;
		HotReloader::IRuntimeObject* obj = 0;
		HotReloader::IRuntimeObject* org = 0;
		bool found = false;

		for(Type::uint32 i = 0; i < size; i++)
		{
			if (db->m_HotReloaderDatabase->m_Objects->At(i).m_TypeID != info->m_TypeID)
			{
				continue;
			}

			found = true;

			Serializer serializer;

			HotReloader::IRuntimeObject* currentObject = db->m_HotReloaderDatabase->m_Objects->At(i).m_Object;
			IReflectionData* type = Reflection::GetType(currentObject->m_TypeID);
			const Vector<IReflectionInheritance>& inhs = type->GetInheritances();
			static size_t _IRuntimeObejctTypeID = Reflection::GetType<HotReloader::IRuntimeObject>()->GetTypeID();
			Variant* generateCastFuncs = type->GetMetaData(Reflection::ReflectionAttribute::GENERATE_CAST_FUNCTIONS);

			org = info->m_CreateObjectCallback(currentObject->m_ObjectID, currentObject->m_ConstructorID);
			obj = org;
			old = currentObject->m_Object;

			for(Type::uint32 i = 0; i < inhs.Size(); i++)
			{
				const IReflectionInheritance* inh = inhs[i];

				if(inh->GetTypeID() == _IRuntimeObejctTypeID && generateCastFuncs->IsValid() && generateCastFuncs->Convert<bool>())
				{
					obj = Reflection::GetType(currentObject->m_TypeID)->Invoke("Cast_IRuntimeObject", obj).Convert<HotReloader::IRuntimeObject*>();
					old = Reflection::GetType(currentObject->m_TypeID)->Invoke("Cast_IRuntimeObject", currentObject->m_Object).Convert<HotReloader::IRuntimeObject*>();

					break;
				}
			}

			old->OnSwap(SwapStage::BEFORE, ObjectAge::OLD);
			obj->OnSwap(SwapStage::BEFORE, ObjectAge::YOUNG);
			m_IsSwapComplete = false;

			serializer.SetIsLoading(false);
			serializer.Serialize(old);

			obj->m_ObjectID = 		currentObject->m_ObjectID;
			obj->m_ConstructorID = 	currentObject->m_ConstructorID;
			obj->m_TypeID = 		currentObject->m_TypeID;
			obj->m_Object = 		org;
			obj->m_Tracker =		currentObject->m_Tracker;

			serializer.SetIsLoading(true);
			serializer.Serialize(obj);

			currentObject->m_Object = org;

			Event::GlobalEvent::Broadcast(InheritanceTrackKeeper::SwapInheritancePointersEvent{});

			old->OnSwap(SwapStage::AFTER, ObjectAge::OLD);
			obj->OnSwap(SwapStage::AFTER, ObjectAge::YOUNG);
			m_IsSwapComplete = true;

			break;
		}

		if(!found)
		{
			size_t objectID = db->GetNextPerObjectID();
			size_t constructorID = Reflection::GetType(info->m_TypeID)->GetConstructor()->GetConstructorID();

			obj = info->m_CreateObjectCallback(
				objectID,
				constructorID
			);

			obj->m_Object = 		obj;
			obj->m_ConstructorID = 	constructorID;
			obj->m_ObjectID = 		objectID;
			obj->m_TypeID = 		info->m_TypeID;

			db->m_HotReloaderDatabase->m_Objects->PushBack(ConstrucedObject(obj->m_ObjectID, constructorID, obj->m_TypeID, new Tuple(std::tuple()), obj));
		}

		obj->OnRecompile(old);

		Event::GlobalEvent::Broadcast(RecompiledEvent{});

		if(info)
		{
			delete info;
			info = 0;
		}

		for(std::string f : m_ObjFilesToDelete)
		{
			std::filesystem::remove(f);
		}

		m_ObjFilesToDelete.clear();
    }

	void RuntimeReloader::operator()(const SwapModuleEvent& event)
	{
		SwapModule(m_BinaryLocation.string() + "/Runtime/Swap/" + event.m_ModuleName + ".dll");
	}

	void RuntimeReloader::operator()(const StopRecompilingEvent& event)
	{
		m_Paused = true;
	}

	void RuntimeReloader::operator()(const StartRecompilingEvent& event)
	{
		m_Paused = false;
	}

}}
#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Engine {

	class EngineEditorApp : public Application
	{
	public:
		EngineEditorApp()
			: Application("EngineEditor")
		{
			PushLayer(new EditorLayer());
		}

		~EngineEditorApp()
		{
		}
	};

	Application* CreateApplication()
	{
		return new EngineEditorApp();
	}

}
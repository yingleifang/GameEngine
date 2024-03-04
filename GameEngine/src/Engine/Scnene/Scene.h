#pragma once

#include "entt.hpp"
#include "Engine/Core/Timestep.h"
#include "Components.h"


class b2World;

namespace Engine {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnRunTimeStart();
		void OnRunTimeStop();

		void OnViewportResize(uint32_t width, uint32_t height);

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;

		b2World* m_PhysicsWorld = nullptr;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		friend class Entity;
	};

}
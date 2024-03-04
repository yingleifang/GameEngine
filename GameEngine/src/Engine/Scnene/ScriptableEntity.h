#pragma once
#include "Entity.h"

namespace Engine
{
	class ScriptableEntity {
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:
		Entity m_Entity;
		friend class Scene;
		void SetEntity(Entity entity) { m_Entity = entity; }
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& e) {}
	};
}
#include "Enginepch.h"
#include "Scene.h"

#include "glm/glm.hpp"
#include "Engine/Renderer/Renderer2D.h"

#include "Components.h"
#include "Entity.h"

#include <box2d/box2d.h>

namespace Engine {
	Scene::Scene()
	{	
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string & name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnRunTimeStart()
	{
		m_PhysicsWorld = new b2World({0.0f, -9.8f});
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto entity : view)
		{
			Entity e = { entity, this };
			auto& transform = e.GetComponent<TransformComponent>();
			auto& rb2d = e.GetComponent<RigidBody2DComponent>();
			b2BodyDef bodyDef;
			switch (rb2d.Type)
			{
			case RigidBody2DComponent::BodyType::Static:
				bodyDef.type = b2_staticBody;
				break;
			case RigidBody2DComponent::BodyType::Dynamic:
				bodyDef.type = b2_dynamicBody;
				break;
			case RigidBody2DComponent::BodyType::Kinematic:
				bodyDef.type = b2_kinematicBody;
				break;
			}
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;	
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (e.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = e.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold =  bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}	
	}

	void Scene::OnRunTimeStop()
	{
		delete m_PhysicsWorld;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}
}
#pragma once
#include "Engine.h"
class Sandbox2d : public Engine::Layer
{
public:
	Sandbox2d();
	virtual ~Sandbox2d() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;
private:
	Engine::OrthographicCameraController m_CameraController;

	std::shared_ptr<Engine::VertexArray> m_SquareVA;
	std::shared_ptr<Engine::Shader> m_FlatColorShader;

	std::shared_ptr<Engine::Scene> m_ActiveScene;

	std::shared_ptr<Engine::Texture2D> m_CheckerboardTexture;

	std::shared_ptr<Engine::Framebuffer> m_Framebuffer;

	glm::vec4 m_SquareColor = { 0.8f, 0.3f, 0.8f, 1.0f };

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	Engine::Entity m_SquareEntity;

	Engine::Entity m_Camera;
};
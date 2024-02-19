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

	glm::vec4 m_SquareColor = { 1.2f, 0.3f, 0.8f, 1.0f };

};
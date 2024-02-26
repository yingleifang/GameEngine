#include "Sandbox2d.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2d::Sandbox2d()
	: Layer("Sandbox2d"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2d::OnAttach()
{
	m_CheckerboardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2d::OnDetach()
{
}

void Sandbox2d::OnUpdate(Engine::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Engine::RenderCommand::Clear();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//Engine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	//Engine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	//Engine::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
	Engine::Renderer2D::DrawRotatedQuad({-2.0f, 0.0f, 0.0f }, { 5.0f, 5.0f }, 45.0f, m_CheckerboardTexture, 20.0f);
	Engine::Renderer2D::EndScene();
}

void Sandbox2d::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2d::OnEvent(Engine::Event& e)
{
	m_CameraController.OnEvent(e);
}
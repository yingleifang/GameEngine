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

	Engine::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	//m_Framebuffer = Engine::Framebuffer::Create(fbSpec);

	m_ActiveScene = std::make_shared<Engine::Scene>();
	ENGINE_INFO(m_ActiveScene.get() == nullptr);
	auto square = m_ActiveScene->CreateEntity("Green Square");

	square.AddComponent<Engine::SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

	m_SquareEntity = square;
}

void Sandbox2d::OnDetach()
{
}

void Sandbox2d::OnUpdate(Engine::Timestep ts)
{


	// Render
	//m_Framebuffer->Bind();
	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Engine::RenderCommand::Clear();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	// Update scene
	m_ActiveScene->OnUpdate(ts);

	Engine::Renderer2D::EndScene();

	//m_Framebuffer->Unbind();
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
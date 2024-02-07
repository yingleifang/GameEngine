
#include "Enginepch.h"
#include "Application.h"
#include "Engine/Log.h"

#include <glad/glad.h>

#include "Input.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

		Application* Application::s_Instance = nullptr;

		Application::Application() : m_Camera(-2.0f, 2.0f, -2.0f, 2.0f) {
			ENGINE_CORE_ASSERT(!s_Instance, "Already has app");
			s_Instance = this;
			m_Window = std::unique_ptr<Window>(Window::Create());
			m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
			m_ImGuiLayer = new ImGuiLayer();
			PushOverlay(m_ImGuiLayer);

			m_VertexArray = VertexArray::Create();

			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
			};

			m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};

			m_VertexBuffer->SetLayout(layout);

			m_VertexArray->AddVertexBuffer(m_VertexBuffer);

			uint32_t indices[3] = { 0, 1, 2 };
			m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			m_VertexArray->SetIndexBuffer(m_IndexBuffer);

			std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

			std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

			m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		}

		Application::~Application() {

		}

		void Application::PushLayer(Layer* layer) {
			m_LayerStack.PushLayer(layer);
			layer->OnAttach();
		}

		void Application::PushOverlay(Layer* layer) {
			m_LayerStack.PushOverlay(layer);
			layer->OnAttach();
		}

		void Application::OnEvent(Event& e) {
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
				(*--it)->OnEvent(e);
				if (e.Handled)
					break;
			}
		}

		void Application::Run() {

			while (m_Running) {
				RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
				RenderCommand::Clear();

				m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
				m_Camera.SetRotation(45.0f);

				Renderer::BeginScene(m_Camera);

				Renderer::Submit(m_VertexArray, m_Shader);

				Renderer::EndScene();

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate();

				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();

				m_Window->OnUpdate();
			}
		}

		bool Application::OnWindowClose(WindowCloseEvent& e) {
			m_Running = false;
			return true;
		}
}
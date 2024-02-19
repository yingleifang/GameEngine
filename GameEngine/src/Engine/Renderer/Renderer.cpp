#include "Enginepch.h"
#include "Renderer.h"
#include <Platform/OpenGl/OpenGLShader.h>
#include "Renderer2D.h"

namespace Engine {
    std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

    void Renderer::Init()
    {
		RenderCommand::Init();
        Renderer2D::Init();
	}

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }
    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
		RenderCommand::SetViewport(0, 0, width, height);
    }
}
#pragma once
#include "LowRenderer/Camera.h"
#include "DataStructure/Graph.h"
#include "CubeRobot.h"
#include "Shader.h"
#include "lib_Export.h"
namespace Resources
{
	class lib_API Scene
	{
	public:
		using Graph = Core::DataStructure::Graph;
		using vec4 = Core::Maths::vec4;
		using vec3 = Core::Maths::vec3;
		using mat4 = Core::Maths::mat4;

		Scene();
		virtual~Scene(void);

		virtual void UpdateScene(float msec);
		virtual void RenderScene();

		void InitScene();

		LowRenderer::Camera m_camera;
		Shader m_shader;
	private:
		mat4 m_projMatrix;
		mat4 m_viewMatrix;
		void DrawNode(Graph& node);
		float m_width = 1250.f;
		float m_height = 600.f;
		Graph* m_root = nullptr;
	};

}
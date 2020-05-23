#include "Resources/Scene.h"
#include "Resources/ObjScene.h"
#include "Maths/mat3.h"
#include "glad/glad.h"
#include <iostream>
namespace Resources
{
	Scene::Scene()
	{
		
		LowRenderer::Camera camera{ Core::Maths::vec3{0.0f, 0.0f, 3.0f} };
		m_camera = camera;
		Resources::Shader ShaderManager{};
		m_shader = ShaderManager;
	}

	Scene::~Scene(void)
	{
		delete m_root;
		CubeRobot::DeleteCube();
		ObjScene::DeleteOBJ();
	}

	void Scene::UpdateScene(float msec)
	{
		m_root->Update(msec);
	}

	void Scene::RenderScene()
	{
		/* Render here */
		glClearColor(0.2f, 0.6f, 0.53f, 1.0f);//Color background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);//Active Depth Buffer
		glUseProgram(m_shader.GetID());

		m_projMatrix = LowRenderer::Camera::CreateProjectionMatrix(m_camera.m_fov, m_width / m_height, 0.1f, 100.0f);
		m_viewMatrix = m_camera.GetViewMatrix();
		DrawNode(*m_root);
	}
	void Scene:: InitScene()
	{
		/*Create a Robot*/
		CubeRobot::CreateCube();

		//Create a Shader Program
		std::string fragmentShaderText = m_shader.LoadFileGLSL("Shader/ShaderFrag.frag");
		std::string vertexShaderText = m_shader.LoadFileGLSL("Shader/ShaderVertex.vert");
		m_shader.CreateShaderProgram(fragmentShaderText.c_str(), vertexShaderText.c_str());

		m_root = new Graph();

		/*Add robot*/
		m_root->AddChild(new CubeRobot());

		/*CreateOBJ*/
		ObjScene::CreateOBJ("resource/Skull.obj", "resource/Skull.jpg");

		/*add OBJ*/
		m_root->AddChild(new ObjScene());

	}

	void Scene::DrawNode(Graph& node)
	{
		if (node.GetModel())
		{
			/*MATRIX*/
			//----------------------------------------------------------------------------------------------
			mat4 model = mat4::Identity();
			model = Resources::Transform::Scale(node.GetWorldTransform(), node.GetModelScale());


			Core::Maths::mat4 MVP = m_projMatrix * m_viewMatrix * model;
			GLuint programID = m_shader.GetID();
			GLint Loc = glGetUniformLocation(programID, "MVP");
			glUniformMatrix4fv(Loc, 1, GL_TRUE, MVP.m_matrix);

			/*Normal Matrix*/
			Loc = glGetUniformLocation(programID, "model");
			glUniformMatrix4fv(Loc, 1, GL_TRUE, model.m_matrix);

			Core::Maths::mat3 normal = Resources::Transform::NormalMatrix(model);
			Loc = glGetUniformLocation(programID, "normalMatrix");
			glUniformMatrix3fv(Loc, 1, GL_TRUE, normal.m_matrix);


			m_shader.SetVec3(m_camera.m_cameraPos, "viewPos", m_shader.GetID());

			//----------------------------------------------------------------------------------------------

			node.Draw();
		}
		for (auto
			i = node.GetChildIteratorStart();
			i != node.GetChildIteratorEnd(); ++i)
		{
			DrawNode(*(*i));
		}

	}
}
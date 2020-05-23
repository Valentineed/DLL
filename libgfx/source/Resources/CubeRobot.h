#pragma once
#include "DataStructure/Graph.h"
#include "Mesh.h"
#include "ResourcesManager.h"
#include "lib_Export.h"
namespace Resources
{
	class CubeRobot : public Core::DataStructure::Graph {
	public:
		using Graph = Core::DataStructure::Graph;
		using vec4 = Core::Maths::vec4;
		using vec3 = Core::Maths::vec3;
		CubeRobot(void);
		~CubeRobot(void) {};
		virtual void Update(float delta);

		static void CreateCube() {

			Mesh mesh(Resources::Mesh::CreateCube());
			LowRenderer::Model* model = new LowRenderer::Model(mesh.GetData());
			unsigned int texture = Resources::ResourcesManager::GetTexture("resource/container.jpg");
			model->SetTextureID(texture);
			s_cube = model;
		}

		
		static void DeleteCube() { delete s_cube; }

	private:
		static LowRenderer::Model* s_cube;
		Graph* m_head;
		Graph* m_leftArm;
		Graph* m_rightArm;

	};
}

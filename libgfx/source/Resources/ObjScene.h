#pragma once
#include "DataStructure/Graph.h"
#include "Mesh.h"
#include "ResourcesManager.h"
namespace Resources
{
	class ObjScene : public Core::DataStructure::Graph {
	public:
		using Graph = Core::DataStructure::Graph;
		using vec4 = Core::Maths::vec4;
		using vec3 = Core::Maths::vec3;
		ObjScene(void);
		~ObjScene(void) {};
		virtual void Update(float msec);

		static void CreateOBJ(const char* modelPath, std::string imgPath) {
			Mesh mesh(modelPath);
			LowRenderer::Model* model = new LowRenderer::Model(mesh.GetData());
			unsigned int texture = Resources::ResourcesManager::GetTexture(imgPath);
			model->SetTextureID(texture);
			s_obj = model;
		}

		static void DeleteOBJ() { delete s_obj; }

	private:
		static LowRenderer::Model* s_obj;
		Graph* m_obj;
	};

	LowRenderer::Model* ObjScene::s_obj = nullptr;
	ObjScene::ObjScene()
	{
		m_obj = new Graph(s_obj, vec4{ 1.f, 0.f, 0.f, 1.f });
		m_obj->SetModelScale(vec3{ 0.25, 0.25, 0.25 });
		m_obj->SetTransform(Transform::Rotate(mat4::Identity(), -90.f, vec3(1.f, 0.f, 0.f)));
		m_obj->SetTransform(Transform::Translate(m_obj->GetTransform(), vec3(-0.5f, 6.f, 8.5f)));
		AddChild(m_obj);
	}

	inline void ObjScene::Update(float msec)
	{
		Graph::Update(msec);
	}

}
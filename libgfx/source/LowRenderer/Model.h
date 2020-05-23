#pragma once
#include "Maths/vec3.h"
#include "Maths/vec2.h"
#include <string>
#include <vector>
namespace LowRenderer
{
	class Model
	{
	public:
		using vec3 = Core::Maths::vec3;
		using vec2 = Core::Maths::vec2;


		struct Vertex
		{
			vec3 m_position;
			vec3 m_color;
			vec3 m_normal;
			vec2 m_texCoord;
			Vertex(float posX, float posY, float posZ, float red, float green, float blue, float normX, float normY, float normZ, float texS, float texT) :
				m_position(posX, posY, posZ),
				m_color(red, green, blue),
				m_normal(normX, normY, normZ),
				m_texCoord(texS, texT)
			{

			};

			Vertex(vec3 const& position, vec3 const& color, vec3 const& normal, vec2 const& texCoord) :
				m_position(position),
				m_color(color),
				m_normal(normal),
				m_texCoord(texCoord)
			{};	
		};
		/*Data for the Model*/
		std::vector<Vertex> m_data;


		/*Functions*/
		Model() = default;
		Model(std::vector<float> const& data);
		void Draw();
		
		
		void SetTextureID(unsigned int const& ID);
		unsigned int GetTextureID()const { return m_textureID; }
		
	/*	~Model() = delete;*/

	private:
		/*Texture*/
		unsigned int m_textureID;

		/*Render Data*/
		unsigned int m_VAO = 0, m_VBO = 0, m_EBO = 0;
		
		/*Functions*/
		void SetupModel(std::vector<float>const& data);
	};
}
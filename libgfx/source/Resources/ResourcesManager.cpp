#include "Resources/ResourcesManager.h"

namespace Resources
{
	ResourcesManager ResourcesManager::m_instance;
	unsigned int ResourcesManager::GetTexture(std::string path)
	{
		return m_instance.m_texManager.GetTextureID(path);
	}
}
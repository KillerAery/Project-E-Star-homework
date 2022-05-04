#include "PBRMesh.h"

Aery::PBRMesh::PBRMesh():
	IMesh(1)
{

}

Aery::PBRMesh::~PBRMesh()
{
}

void Aery::PBRMesh::LoadMesh(
	const char* meshName, 
	const char* baseColorMapName, 
	const char* normalMapName, 
	const char* aromMapName)
{
	m_mesh = meshLoad(meshName);
	m_textureColor = loadTexture(baseColorMapName);
	m_textureNormal = loadTexture(normalMapName);
	m_textureAorm = loadTexture(aromMapName);
}

void Aery::PBRMesh::destory()
{
	if (m_mesh)meshUnload(m_mesh);
	bgfx::destroy(m_textureColor);
	bgfx::destroy(m_textureNormal);
	bgfx::destroy(m_textureAorm);
}

bgfx::TextureHandle Aery::PBRMesh::getTexColor()
{
	return m_textureColor;
}

bgfx::TextureHandle Aery::PBRMesh::getTexNormal()
{
	return m_textureNormal;
}

bgfx::TextureHandle Aery::PBRMesh::getTexAorm()
{
	return m_textureAorm;
}

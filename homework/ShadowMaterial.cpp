#include "ShadowMaterial.h"

Aery::ShadowMaterial::ShadowMaterial():
	m_shadowmapSize(512)
{
}

Aery::ShadowMaterial::~ShadowMaterial()
{
}

void Aery::ShadowMaterial::init()
{
	m_program = loadProgram("vs_shadow", "fs_shadow");

	bgfx::TextureHandle fbtextures[] =
	{
		bgfx::createTexture2D(
			  m_shadowmapSize
			, m_shadowmapSize
			, false
			, 1
			, bgfx::TextureFormat::D16
			, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
			),
	};
	m_shadowmap = fbtextures[0];
	m_shadowmapFB = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
}

void Aery::ShadowMaterial::destory()
{
	bgfx::destroy(m_program);
}

void Aery::ShadowMaterial::setState()
{
}

void Aery::ShadowMaterial::drawMesh(IMesh& mesh, float* model)
{
	if (mesh.getType() == 0) {
		bgfx::submit(0, m_program);
	}
	else {
		meshSubmit(mesh.getMesh(), 1, m_program, model);
	}
}

uint16_t Aery::ShadowMaterial::getShadowmapSize() const
{
	return m_shadowmapSize;
}

bgfx::TextureHandle Aery::ShadowMaterial::getShadowmap()
{
	return m_shadowmap;
}

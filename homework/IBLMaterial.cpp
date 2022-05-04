#include "IBLMaterial.h"

Aery::IBLMaterial::IBLMaterial()
{
}

Aery::IBLMaterial::~IBLMaterial()
{

}

void Aery::IBLMaterial::init()
{
	m_program = loadProgram("vs_IBL", "fs_IBL");
	m_texLUT = loadTexture("../resource/GGX_E_LUT.dds");
	m_programWithShadow = loadProgram("vs_IBL_withShadow", "fs_IBL_withShadow");
}

void Aery::IBLMaterial::destory()
{
	bgfx::destroy(m_program);
	bgfx::destroy(m_texLUT);
}

void Aery::IBLMaterial::setState()
{
	bgfx::setState(
		BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_CULL_CW
		| BGFX_STATE_MSAA
	);
}

void Aery::IBLMaterial::drawMesh(IMesh& mesh, float* model, int modelNums)
{
	if (mesh.getType() == 1) {
		if (shadowOn) {
			meshSubmit(mesh.getMesh(), 0, m_programWithShadow, model);
		}
		else {
			meshSubmit(mesh.getMesh(), 0, m_program, model);
		}
	}
}

bgfx::TextureHandle Aery::IBLMaterial::getTexLUT()
{
	return m_texLUT;
}

void Aery::IBLMaterial::setShadowOn(bool shadowOn)
{
	this->shadowOn = shadowOn;
}

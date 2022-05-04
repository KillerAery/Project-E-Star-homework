#include "PBRMaterial.h"

Aery::PBRMaterial::PBRMaterial():
	shadowOn(false)
{
}

Aery::PBRMaterial::~PBRMaterial()
{
}

void Aery::PBRMaterial::init()
{
	m_program = loadProgram("vs_PBR", "fs_PBR");
	m_programWithShadow = loadProgram("vs_PBR_withShadow", "fs_PBR_withShadow");
}

void Aery::PBRMaterial::destory()
{
	bgfx::destroy(m_program);
	bgfx::destroy(m_programWithShadow);
}

void Aery::PBRMaterial::setState()
{
	uint64_t state = 0
		| BGFX_STATE_WRITE_R
		| BGFX_STATE_WRITE_G
		| BGFX_STATE_WRITE_B
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_CULL_CW
		| BGFX_STATE_MSAA
		;
	bgfx::setState(state);
}

void Aery::PBRMaterial::drawMesh(IMesh& mesh, float* model)
{
	if (mesh.getType() == 0) {
		//do nothing
	}
	else {
		if (shadowOn) {
			meshSubmit(mesh.getMesh(), 0, m_programWithShadow, model);
		}
		else {
			meshSubmit(mesh.getMesh(), 0, m_program, model);
		}
	}
}

void Aery::PBRMaterial::setShadowOn(bool shadowOn)
{
	this->shadowOn = shadowOn;
}

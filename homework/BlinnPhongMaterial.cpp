#include "BlinnPhongMaterial.h"

Aery::BlinnPhongMaterial::BlinnPhongMaterial()
{
}

Aery::BlinnPhongMaterial::~BlinnPhongMaterial()
{
}

void Aery::BlinnPhongMaterial::init()
{
	m_program = loadProgram("vs_blinnphong", "fs_blinnphong");
	m_programWithShadow = loadProgram("vs_blinnphong_withShadow", "fs_blinnphong_withShadow");
}

void Aery::BlinnPhongMaterial::destory()
{
	bgfx::destroy(m_program);
}

void Aery::BlinnPhongMaterial::setState()
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

void Aery::BlinnPhongMaterial::drawMesh(IMesh& mesh, float* model, int modelNums)
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

void Aery::BlinnPhongMaterial::setShadowOn(bool shadowOn)
{
	this->shadowOn = shadowOn;
}

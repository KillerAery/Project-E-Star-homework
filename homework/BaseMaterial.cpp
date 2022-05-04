#include "BaseMaterial.h"

Aery::BaseMaterial::BaseMaterial()
{
}

Aery::BaseMaterial::~BaseMaterial()
{
}

void Aery::BaseMaterial::init()
{
	m_program = loadProgram("vs_cubes", "fs_cubes");
}

void Aery::BaseMaterial::destory()
{
	bgfx::destroy(m_program);
}

void Aery::BaseMaterial::setState()
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
		| UINT64_C(0)
		;

	bgfx::setState(state);
}

void Aery::BaseMaterial::drawMesh(IMesh& mesh, float* model, int modelNums)
{
	if (mesh.getType() == 0) {
		bgfx::setTransform(model);
		bgfx::submit(0, m_program);
	}
	else {
		meshSubmit(mesh.getMesh(), 0, m_program, model);
	}
}

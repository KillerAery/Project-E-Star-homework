#include "SkyMaterial.h"

Aery::SkyMaterial::SkyMaterial()
{
}

Aery::SkyMaterial::~SkyMaterial()
{
}

void Aery::SkyMaterial::destory()
{
	bgfx::destroy(m_program);
}

void Aery::SkyMaterial::init()
{
	m_program = loadProgram("vs_skybox", "fs_skybox");
}

void Aery::SkyMaterial::setState()
{
	bgfx::setState(
		BGFX_STATE_WRITE_RGB
		| BGFX_STATE_DEPTH_TEST_EQUAL
	);
}

void Aery::SkyMaterial::drawMesh(IMesh& mesh, float* model)
{
	if (mesh.getType() == 2) {
		bgfx::submit(2, m_program);
	}
	else {
		//do nothing
	}
}

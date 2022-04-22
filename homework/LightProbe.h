#pragma once

#include "bgfx_utils.h"

struct LightProbe
{
	void load(const char* _name)
	{
		char filePath[512];

		bx::snprintf(filePath, BX_COUNTOF(filePath), "../resource/env_maps/%s_lod.dds", _name);
		m_tex = loadTexture(filePath, BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);

		bx::snprintf(filePath, BX_COUNTOF(filePath), "../resource/env_maps/%s_irr.dds", _name);
		m_texIrr = loadTexture(filePath, BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
	}

	void destroy()
	{
		bgfx::destroy(m_tex);
		bgfx::destroy(m_texIrr);
	}

	bgfx::TextureHandle m_tex;
	bgfx::TextureHandle m_texIrr;
};
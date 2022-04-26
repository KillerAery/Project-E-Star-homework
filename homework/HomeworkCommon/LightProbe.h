#pragma once

#include "bgfx_utils.h"

namespace Aery {
	struct LightProbe
	{
		void load(const char* _name)
		{
			char filePath[512];

			bx::snprintf(filePath, BX_COUNTOF(filePath), "../resource/env_maps/%s_lod.dds", _name);
			texLOD = loadTexture(filePath, BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);

			bx::snprintf(filePath, BX_COUNTOF(filePath), "../resource/env_maps/%s_irr.dds", _name);
			texIrr = loadTexture(filePath, BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
		}

		void destroy()
		{
			bgfx::destroy(texLOD);
			bgfx::destroy(texIrr);
		}

		bgfx::TextureHandle texLOD;
		bgfx::TextureHandle texIrr;
	};
}
#pragma once

#include "bgfx_utils.h"

namespace Aery {
	class LightProbe
	{
	public:
		LightProbe() {

		}

		~LightProbe() {
		}

		void load(const char* _name)
		{
			char filePath[512];

			bx::snprintf(filePath, BX_COUNTOF(filePath), "../resource/env_maps/%s_lod.dds", _name);
			m_texLOD = loadTexture(filePath, BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);

			bx::snprintf(filePath, BX_COUNTOF(filePath), "../resource/env_maps/%s_irr.dds", _name);
			m_texIrr = loadTexture(filePath, BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
		}

		void destroy()
		{
			bgfx::destroy(m_texLOD);
			bgfx::destroy(m_texIrr);
		}

		bgfx::TextureHandle getTexLOD() {
			return m_texLOD;
		}		
		
		bgfx::TextureHandle getTexIrr() {
			return m_texIrr;
		}

	private:
		bgfx::TextureHandle m_texLOD;
		bgfx::TextureHandle m_texIrr;
	};
}
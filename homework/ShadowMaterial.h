#pragma once

#include "IMaterial.h"

namespace Aery {
	class ShadowMaterial :public IMaterial {
	public:
		ShadowMaterial();

		virtual ~ShadowMaterial();

		void init() override;

		void destory() override;

		void setState() override;

		void drawMesh(IMesh& mesh, float* model)override;

		uint16_t getShadowmapSize()const;

		bgfx::TextureHandle getShadowmap();
	private:
		uint16_t m_shadowmapSize;

		bgfx::ProgramHandle m_program;
		bgfx::TextureHandle m_shadowmap;
		bgfx::FrameBufferHandle m_shadowmapFB;
	};
};
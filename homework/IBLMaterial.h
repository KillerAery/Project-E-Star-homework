#pragma once

#include "IMaterial.h"

namespace Aery {
	class IBLMaterial :public IMaterial {
	public:
		IBLMaterial();

		virtual ~IBLMaterial();

		void init() override;

		void destory() override;

		void setState() override;

		void drawMesh(IMesh& mesh, float* model)override;

		bgfx::TextureHandle getTexLUT();

	private:
		bgfx::ProgramHandle m_program;
		bgfx::TextureHandle m_texLUT;
	};
};
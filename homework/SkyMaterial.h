#pragma once

#include "IMaterial.h"

namespace Aery {

	class SkyMaterial : public IMaterial {
	public:
		SkyMaterial();

		virtual ~SkyMaterial();

		void destory() override;

		void init() override;

		void setState() override;

		void drawMesh(IMesh& mesh, float* model = nullptr, int modelNums = 1)override;

	private:
		bgfx::ProgramHandle m_program;
	};

};

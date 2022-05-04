#pragma once

#include "IMaterial.h"

namespace Aery {
	class BlinnPhongMaterial :public IMaterial {
	public:
		BlinnPhongMaterial();

		virtual ~BlinnPhongMaterial();

		void init() override;

		void destory() override;

		void setState() override;

		void drawMesh(IMesh& mesh, float* model)override;

	private:
		bgfx::ProgramHandle m_program;
	};
};
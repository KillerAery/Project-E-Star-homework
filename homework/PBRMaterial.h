#pragma once

#include "IMaterial.h"

namespace Aery {
	class PBRMaterial :public IMaterial {
	public:
		PBRMaterial();

		virtual ~PBRMaterial();

		void init() override;

		void destory() override;

		void setState() override;

		void drawMesh(IMesh& mesh, float* model, int modelNums = 1)override;

		void setShadowOn(bool shadowOn);

	private:
		bgfx::ProgramHandle m_program;
		bgfx::ProgramHandle m_programWithShadow;
		bool shadowOn;
	};
};
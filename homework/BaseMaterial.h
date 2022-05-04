#pragma once

#include "IMaterial.h"

namespace Aery {

	class BaseMaterial : public IMaterial {
	public:
		BaseMaterial();

		virtual ~BaseMaterial(); 

		void init() override;

		void destory() override;

		void setState() override;

		void drawMesh(IMesh& mesh, float* model, int modelNums = 1)override;

	private:
		bgfx::ProgramHandle m_program;
	};

};

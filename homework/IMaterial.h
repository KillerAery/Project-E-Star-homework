#pragma once

#include "bgfx_utils.h"
#include "IMesh.h"

namespace Aery {
	class IMaterial
	{
	public:
		IMaterial() {}

		virtual ~IMaterial() {}

		virtual void init() = 0;

		virtual void destory() = 0;

		virtual void setState() = 0;

		virtual void drawMesh(IMesh& mesh, float* model = nullptr,int modelNums=1) = 0;
	};
};

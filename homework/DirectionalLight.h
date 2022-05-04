#pragma once

#include "bgfx_utils.h"

namespace Aery {
	struct DirectionalLight
	{
		void updateDirection() {
			//direction = bx::normalize(bx::sub(at,pos));
			direction = bx::normalize(direction);
			pos = mul(direction,50);
		}

		void updateRadiance(float multi = 1.0f) {
			radiance[0] = radiance_colorWheel[0] * multi;
			radiance[1] = radiance_colorWheel[1] * multi;
			radiance[2] = radiance_colorWheel[2] * multi;
			radiance[3] = radiance_colorWheel[3] * multi;
		}

		void updateView() {
			bx::mtxLookAt(view, pos, at);
		}

		void updateProj(float area = 200.0f) {
			bx::mtxOrtho(proj, -area, area, -area, area, 0.1f, 300.0f, 0.0f, 
				bgfx::getCaps()->homogeneousDepth);
		}

		void updateLightMtx() {
			const bgfx::Caps* caps = bgfx::getCaps();
			const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
			const float sz = caps->homogeneousDepth ? 0.5f : 1.0f;
			const float tz = caps->homogeneousDepth ? 0.5f : 0.0f;
			const float mtxCrop[16] =
			{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f,   sy, 0.0f, 0.0f,
				0.0f, 0.0f, sz,   0.0f,
				0.5f, 0.5f, tz,   1.0f,
			};

			float mtxTmp[16];
			bx::mtxMul(mtxTmp, proj, mtxCrop);
			bx::mtxMul(lightMtx, view, mtxTmp);
		}

		bx::Vec3 direction = { 0.5f,0.5f,-0.5f };
		bx::Vec3 pos = { 10.0f,10.0f,0.0f };
		bx::Vec3 at = { 0.0f,-20.0f,0.0f };

		float radiance_colorWheel[4] = {1.0f,0.8f,0.2f,1.0f};
		float radiance[4];
		float view[16];
		float proj[16];
		float lightMtx[16];
	};
}
#pragma once

#include "bgfx_utils.h"

namespace Aery {
	struct DirectionalLight
	{
		void moveLight(float x, float y, float z) {
			pos = { x,y,z };
		}

		void updateDirection() {
			direction = bx::sub(at,pos);
			direction = bx::normalize(direction);
		}

		void updateRadiance() {
			radiance[0] = radiance_colorWheel[0] * 10;
			radiance[1] = radiance_colorWheel[1] * 10;
			radiance[2] = radiance_colorWheel[2] * 10;
			radiance[3] = radiance_colorWheel[3] * 10;
		}

		void updateView() {
			bx::mtxLookAt(view, pos, at);
		}

		void updateProj(float area = 30.0f) {
			bx::mtxOrtho(proj, -area, area, -area, area, -100.0f, 100.0f, 0.0f, 
				bgfx::getCaps()->homogeneousDepth);
		}

		void updateMVP() {
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

			float temp[16];
			bx::mtxMul(temp, proj, mtxCrop);
			float temp2[16];
			bx::mtxMul(temp2, view, temp);

			bx::mtxTranslate(model, pos.x, pos.y, pos.z);
			bx::mtxMul(MVP, model,temp2);
		}

		bx::Vec3 pos = { 70.0f,60.0f,80.0f };
		bx::Vec3 at = { 0.0f,0.0f,0.0f };
		bx::Vec3 direction = { 0,0,0 };
		float radiance_colorWheel[4] = {1.0f,0.8f,0.2f,1.0f};
		float radiance[4];
		float model[16];
		float view[16];
		float proj[16];
		float MVP[16];
	};
}
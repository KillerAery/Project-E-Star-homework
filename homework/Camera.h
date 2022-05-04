#pragma once

#include "bgfx_utils.h"

namespace Aery {
	struct Camera
	{
		void changeRadiusScale(float scale) {
			cameraRadius = 35.0f * scale;
		}

		void moveCamera(float moveForward, float moveLeft)
		{
			at = bx::add(at,
				bx::add(bx::mul(cameraRight, moveForward),
					bx::mul(cameraForward, moveLeft))
			);
			cameraForward = bx::sub(at, pos);
			cameraRight = bx::cross(cameraForward, bx::Vec3{ 0,1,0 });
		}

		void rotateCamera(float rotateX, float rotateY) {
			const float horizonRadius = bx::cos(rotateY);
			pos = bx::add(
				{cameraRadius * bx::sin(rotateX) * horizonRadius,
				cameraRadius * bx::sin(rotateY),
				cameraRadius * bx::cos(rotateX) * horizonRadius}
			,at);
		}

		void updateView() {
			bx::mtxLookAt(view,pos,at);
			bx::mtxInverse(view_inv, view);
		}

		// isProj = true 生成透视投影矩阵 ，否则生成正交投影矩阵
		void updateProj(bool isProj,float screenWidth=0.0f,float screenHeight=0.0f) {
			if (isProj) {
				bx::mtxProj(proj, 60.0f, float(screenWidth) / float(screenHeight), 0.1f, 400.0f, bgfx::getCaps()->homogeneousDepth);
			}
			else {
				bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0, bgfx::getCaps()->homogeneousDepth);
			}
		}

		float cameraRadius = 35.0f;	// Camera 环绕目标的距离
		bx::Vec3 pos = { 0,0,0 };
		bx::Vec3 at = { 0,0,0 };
		bx::Vec3 cameraForward = { 0,0,0 };
		bx::Vec3 cameraRight = { 0,0,0 };
		float view[16];
		float proj[16];
		float view_inv[16];
	};
}

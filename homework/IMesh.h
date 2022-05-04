#pragma once

#include "bgfx_utils.h"

namespace Aery {
	class IMesh {
	public:
		IMesh(int type) :m_meshType(type),m_mesh(nullptr){}

		virtual ~IMesh() {}

		// 0为 CubeMesh，1为 PBRStoneMesh, 2为SkyboxMesh
		int getType()const {
			return m_meshType;
		}

		virtual void destory()=0;

		// 加载外部模型才可用（如PBRStoneMesh）
		Mesh* getMesh()const {
			return m_mesh;
		}
	protected:
		int m_meshType;
		Mesh* m_mesh;
	};
};
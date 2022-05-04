#pragma once

#include "bgfx_utils.h"

namespace Aery {
	class IMesh {
	public:
		IMesh(int type);
		virtual ~IMesh();

		// 0为 CubeMesh，1为 PBRStoneMesh, 2为SkyboxMesh
		int getType()const;

		virtual void destory()=0;

		// 加载外部模型才可用（如PBRStoneMesh）
		Mesh* getMesh()const;
	protected:
		int m_meshType;
		Mesh* m_mesh;
	};
};
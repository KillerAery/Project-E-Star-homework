#pragma once

#include "bgfx_utils.h"

namespace Aery {
	class IMesh {
	public:
		IMesh(int type);
		virtual ~IMesh();

		// 0Ϊ CubeMesh��1Ϊ PBRStoneMesh, 2ΪSkyboxMesh
		int getType()const;

		virtual void destory()=0;

		// �����ⲿģ�Ͳſ��ã���PBRStoneMesh��
		Mesh* getMesh()const;
	protected:
		int m_meshType;
		Mesh* m_mesh;
	};
};
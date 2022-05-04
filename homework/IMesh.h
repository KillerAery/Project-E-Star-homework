#pragma once

#include "bgfx_utils.h"

namespace Aery {
	class IMesh {
	public:
		IMesh(int type) :m_meshType(type),m_mesh(nullptr){}

		virtual ~IMesh() {}

		// 0Ϊ CubeMesh��1Ϊ PBRStoneMesh, 2ΪSkyboxMesh
		int getType()const {
			return m_meshType;
		}

		virtual void destory()=0;

		// �����ⲿģ�Ͳſ��ã���PBRStoneMesh��
		Mesh* getMesh()const {
			return m_mesh;
		}
	protected:
		int m_meshType;
		Mesh* m_mesh;
	};
};
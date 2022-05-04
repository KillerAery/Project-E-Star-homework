#pragma once
#include "InputLayout.h"
#include "IMesh.h"

namespace Aery {
	class CubeMesh : public IMesh {
	public:
		CubeMesh(); 
		virtual ~CubeMesh();		
		
		// Ìí¼Ó Mesh µ½¶¥µã»º´æ£¬Ë÷Òý»º´æ
		void AddStaticMesh(bgfx::VertexBufferHandle& vbh, bgfx::IndexBufferHandle& ibh);

		void destory() override;
	private:
		PosColorVertex s_cubeVertices[8] =
		{
			{-1.0f,  1.0f,  1.0f, 0xfff00fff },
			{ 1.0f,  1.0f,  1.0f, 0xfff00fff },
			{-1.0f, -1.0f,  1.0f, 0xfff00000 },
			{ 1.0f, -1.0f,  1.0f, 0xfff00000 },
			{-1.0f,  1.0f, -1.0f, 0x00000fff },
			{ 1.0f,  1.0f, -1.0f, 0x00000fff },
			{-1.0f, -1.0f, -1.0f, 0x00ffff00 },
			{ 1.0f, -1.0f, -1.0f, 0x00ffff00 },
		};

		uint16_t s_cubeTriList[36] =
		{
			0, 1, 2, // 0
			1, 3, 2,
			4, 6, 5, // 2
			5, 6, 7,
			0, 2, 4, // 4
			4, 2, 6,
			1, 5, 3, // 6
			5, 7, 3,
			0, 4, 1, // 8
			4, 5, 1,
			2, 3, 6, // 10
			6, 3, 7,
		};
	};
}
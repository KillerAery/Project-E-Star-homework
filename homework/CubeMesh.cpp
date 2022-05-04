#include "CubeMesh.h"

Aery::CubeMesh::CubeMesh():
	IMesh(0)
{
}

Aery::CubeMesh::~CubeMesh()
{
}

// Ìí¼Ó Mesh µ½¶¥µã»º´æ£¬Ë÷Òý»º´æ

void Aery::CubeMesh::AddStaticMesh(bgfx::VertexBufferHandle& vbh, bgfx::IndexBufferHandle& ibh) {
	// Create static vertex buffer.
	vbh = bgfx::createVertexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
		, PosColorVertex::ms_layout
	);

	// Create static index buffer for triangle list rendering.
	ibh = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
	);
}

void Aery::CubeMesh::destory()
{
}

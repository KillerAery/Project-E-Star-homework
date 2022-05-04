#include "SkyboxMesh.h"
#include <iostream>
#include "InputLayout.h"

Aery::SkyboxMesh::SkyboxMesh():
	IMesh(2)
{
}

Aery::SkyboxMesh::~SkyboxMesh()
{
}

void Aery::SkyboxMesh::AddMesh(
	float _textureWidth, 
	float _textureHeight, 
	bool _originBottomLeft,
	float _width, 
	float _height
)
{
	if (3 != bgfx::getAvailTransientVertexBuffer(3, PosTexcoordVertex::ms_layout))return;
	bgfx::TransientVertexBuffer vb;
	bgfx::allocTransientVertexBuffer(&vb, 3, PosTexcoordVertex::ms_layout);
	PosTexcoordVertex* vertex = (PosTexcoordVertex*)vb.data;

	const float zz = 100.0f;

	const float minx = -_width;
	const float maxx = _width;
	const float miny = 0.0f;
	const float maxy = _height * 2.0f;

	static float s_texelHalf = 0.0f;
	const float texelHalfW = s_texelHalf / _textureWidth;
	const float texelHalfH = s_texelHalf / _textureHeight;
	const float minu = -1.0f + texelHalfW;
	const float maxu = 1.0f + texelHalfW;

	float minv = texelHalfH;
	float maxv = 2.0f + texelHalfH;

	if (_originBottomLeft)
	{
		std::swap(minv, maxv);
		minv -= 1.0f;
		maxv -= 1.0f;
	}

	vertex[0].m_x = minx;
	vertex[0].m_y = miny;
	vertex[0].m_z = zz;
	vertex[0].m_u = minu;
	vertex[0].m_v = minv;

	vertex[1].m_x = maxx;
	vertex[1].m_y = miny;
	vertex[1].m_z = zz;
	vertex[1].m_u = maxu;
	vertex[1].m_v = minv;

	vertex[2].m_x = maxx;
	vertex[2].m_y = maxy;
	vertex[2].m_z = zz;
	vertex[2].m_u = maxu;
	vertex[2].m_v = maxv;

	bgfx::setVertexBuffer(0, &vb);
}

void Aery::SkyboxMesh::destory()
{
}

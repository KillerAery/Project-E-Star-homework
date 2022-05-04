#pragma once

#include "bgfx_utils.h"

namespace Aery {
	struct PosColorVertex {
		float m_x;
		float m_y;
		float m_z;
		uint32_t m_abgr;
		static void init() {
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
				.end();
		};
		static bgfx::VertexLayout ms_layout;
	};

	struct PosNormalTexcoordVertex
	{
		float m_x;
		float m_y;
		float m_z;
		int16_t m_u;
		int16_t m_v;
		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
				.end();
		};
		static bgfx::VertexLayout ms_layout;
	};

	struct PosTangentNormalTexcoordVertex
	{
		float m_x, m_y, m_z;
		float m_tx, m_ty, m_tz, m_tw;
		float m_nx, m_ny, m_nz, m_nw;
		float m_u, m_v;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.end();
		};
		static bgfx::VertexLayout ms_layout;
	};

	struct PosTexcoordVertex
	{
		float m_x, m_y, m_z;
		float m_u, m_v;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.end();
		};
		static bgfx::VertexLayout ms_layout;
	};

};

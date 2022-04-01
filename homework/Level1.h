#pragma once

#include "common.h"
#include "bgfx_utils.h"

class Level1 : public entry::AppI {

public:
	Level1(const char* _name, const char* _description, const char* _url);

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override;

	virtual int shutdown() override;

	bool update() override;

private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	// 绘制相关
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	bgfx::ProgramHandle m_program;
	int64_t m_timeOffset;

	// 输入相关
	entry::MouseState m_mouseState;
	float m_mousex = 0.0f;
	float m_mousey = 0.0f;

public:
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
	static PosColorVertex s_cubeVertices[];
	static const uint16_t s_cubeTriList[];
	static const uint64_t s_ptState;
};
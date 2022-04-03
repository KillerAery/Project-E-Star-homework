#pragma once

#include "common.h"
#include "bgfx_utils.h"


class Level2 : public entry::AppI {

public:
	Level2(const char* _name, const char* _description, const char* _url);

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override;

	virtual int shutdown() override;

	bool update() override;

private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
	
	// 相机
	bx::Vec3 m_cameraForward = { 0,0,0 };
	bx::Vec3 m_cameraRight = { 0,0,0 };
	bx::Vec3 m_cameraOffset = { 0,0,0 };

	// 模型
	Mesh* m_mesh;

	// 绘制相关
	bgfx::TextureHandle m_textureColor;
	bgfx::UniformHandle s_texColor;

	bgfx::UniformHandle u_k;
	bgfx::UniformHandle u_lightPos;
	bgfx::UniformHandle u_ambientIntensity;
	bgfx::UniformHandle u_lightIntensity;

	bgfx::ProgramHandle m_program;

	int64_t m_timeOffset;

	// 输入相关
	entry::MouseState m_mouseState;		
	float m_mousex = 0.0f;
	float m_mousey = 0.0f;

private:
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
};
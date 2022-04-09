#pragma once

#include "common.h"
#include "bgfx_utils.h"


class Level3 : public entry::AppI {

public:
	Level3(const char* _name, const char* _description, const char* _url);

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override;

	virtual int shutdown() override;

	bool update() override;

private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
	
	// ���
	bx::Vec3 m_cameraForward = { 0,0,0 };
	bx::Vec3 m_cameraRight = { 0,0,0 };
	bx::Vec3 m_cameraOffset = { 0,0,0 };

	// ģ��
	Mesh* m_mesh;

	// �������
	bgfx::TextureHandle m_textureColor;
	bgfx::UniformHandle s_texColor;
	bgfx::TextureHandle m_textureNormal;
	bgfx::UniformHandle s_texNormal;
	bgfx::TextureHandle m_textureAorm;
	bgfx::UniformHandle s_texAorm;

	bgfx::UniformHandle u_eyePos;
	bgfx::UniformHandle u_lightDir;
	bgfx::UniformHandle u_lightRadiance;
	bgfx::UniformHandle u_ambient;

	bgfx::ProgramHandle m_program;

	int64_t m_timeOffset;
	float m_lightDir[4] = { 0.0f,1.0f ,0.0f };
	float m_lightRadiance[4] = { 0.2f,0.5f,0.2f };
	float m_ambient[4] = { 0.4f,0.4f,0.4f };

	// �������
	entry::MouseState m_mouseState;		
	float m_mousex = 0.0f;
	float m_mousey = 0.0f;

private:
	struct PosTBNTexcoord3Vertex
	{
		float m_x;
		float m_y;
		float m_z;
		float m_tx;
		float m_ty;
		float m_tz;
		float m_nx;
		float m_ny;
		float m_nz;
		int16_t m_u;
		int16_t m_v;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
				.end();
		};
		static bgfx::VertexLayout ms_layout;
	};
};
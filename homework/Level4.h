#pragma once

#include "common.h"
#include "bgfx_utils.h"
#include "HomeworkCommon/LightProbe.h"
#include "HomeworkCommon/Camera.h"

using namespace Aery;

class Level4 : public entry::AppI {

public:
	Level4(const char* _name, const char* _description, const char* _url);

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override;

	virtual int shutdown() override;

	bool update() override;

	// 更新 UI
	void updateImgui();

	// 更新输入
	void updateInput();

	// 绘制 PBR_Stone：使用IBL shader
	void drawPBRStone();

	// 绘制 Skybox：使用 skybox shader
	void drawSkybox();
private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	Camera m_camera;	// 相机
	Mesh* m_mesh;		// 模型
	LightProbe m_lightProbe;	// 环境光probe

	// 绘制相关
	bgfx::TextureHandle m_textureColor;
	bgfx::UniformHandle s_texColor;
	bgfx::TextureHandle m_textureNormal;
	bgfx::UniformHandle s_texNormal;
	bgfx::TextureHandle m_textureAorm;
	bgfx::UniformHandle s_texAorm;
	bgfx::UniformHandle s_texCube;
	bgfx::UniformHandle s_texCubeIrr;
	bgfx::TextureHandle m_texLUT;
	bgfx::UniformHandle s_texLUT;

	bgfx::UniformHandle u_eyePos;
	bgfx::UniformHandle u_lightDir;
	bgfx::UniformHandle u_lightRadiance;
	bgfx::UniformHandle u_k;
	bgfx::UniformHandle u_mtx;

	bgfx::ProgramHandle m_programIBL;
	bgfx::ProgramHandle m_programSkybox;

	int64_t m_timeOffset;
	float m_lightDir[4] = { -0.5f,-0.5f ,0.5f };
	float m_lightRadiance[4] = { 0.0f,0.0f,0.0f };
	float m_k[4] = {0.6f,0.4f};
	float m_mtx[16] = { 0.0f };
	float m_envRotCurr = 0.0f;
	float m_envRotDest = 0.0f;

	// 输入相关
	entry::MouseState m_mouseState;		
	float m_mousex = 0.0f;
	float m_mousey = 0.0f;
private:
	struct PosTangentNormalTexcoordVertex
	{
		float m_x, m_y, m_z;
		float m_tx, m_ty, m_tz;
		float m_nx, m_ny, m_nz;
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
private:
	// 装配屏幕空间quad顶点
	void screenSpaceQuad(
		float _textureWidth, float _textureHeight, bool _originBottomLeft = false,
		float _width = 1.0f, float _height = 1.0f
	);
};
#pragma once

#include "common.h"
#include "bgfx_utils.h"
#include "HomeworkCommon/LightProbe.h"
#include "HomeworkCommon/Camera.h"
#include "HomeworkCommon/DirectionalLight.h"

using namespace Aery;

class Level5 : public entry::AppI {

public:
	Level5(const char* _name, const char* _description, const char* _url);

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override;

	virtual int shutdown() override;

	bool update() override;

	// 更新 UI
	void updateImgui();

	// 更新输入
	void updateInput();

	// 更新灯光
	void updateLight();

	// 绘制 shadowmap
	void drawShadowmap();

	// 绘制 PBR_Stone：使用 PBRwithShadow shader
	void drawPBRStone();

	// 绘制 cubes：使用 blinnphongwithShadow shader
	void drawCubes();
private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	Camera m_camera;	// 相机
	Mesh* m_mesh;		// 模型
	DirectionalLight m_light;	// 方向光源

	// 绘制相关
	bgfx::TextureHandle m_textureColor;
	bgfx::UniformHandle s_texColor;
	bgfx::TextureHandle m_textureNormal;
	bgfx::UniformHandle s_texNormal;
	bgfx::TextureHandle m_textureAorm;
	bgfx::UniformHandle s_texAorm;
	bgfx::TextureHandle m_shadowmap;
	bgfx::UniformHandle s_shadowmap;

	bgfx::UniformHandle u_eyePos;
	bgfx::UniformHandle u_lightDir;
	bgfx::UniformHandle u_lightRadiance;
	bgfx::UniformHandle u_k;
	bgfx::UniformHandle u_ambient;
	bgfx::UniformHandle u_lightMVP; 
	bgfx::UniformHandle u_depthScaleOffset;

	bgfx::ProgramHandle m_programShadow;
	bgfx::ProgramHandle m_programPBR;
	bgfx::FrameBufferHandle m_shadowmapFB;

	int64_t m_timeOffset;
	float m_k[4] = {0.5f,0.8f,0.02f};
	uint16_t m_shadowmapSize = 512;
	float m_ambient[4] = {1.f,1.f,1.f,0};

	// 输入相关
	entry::MouseState m_mouseState;		
	float m_mousex = 0.0f;
	float m_mousey = 0.0f;
private:
	struct PosTangentNormalTexcoordVertex
	{
		float m_x, m_y, m_z;
		float m_tx, m_ty, m_tz,m_tw;
		float m_nx, m_ny, m_nz,m_nw;
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
	struct PosVertex
	{
		float m_x, m_y, m_z;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.end();
		};
		static bgfx::VertexLayout ms_layout;
	};
};
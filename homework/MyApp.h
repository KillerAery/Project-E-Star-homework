#pragma once

#include "common.h"
#include "bgfx_utils.h"
#include "LightProbe.h"
#include "Camera.h"
#include "DirectionalLight.h"

#include "CubeMesh.h"
#include "PBRMesh.h"
#include "SkyboxMesh.h"

#include "BaseMaterial.h"
#include "BlinnPhongMaterial.h"
#include "PBRMaterial.h"
#include "IBLMaterial.h"
#include "ShadowMaterial.h"
#include "SkyMaterial.h"

using namespace Aery;

class MyApp : public entry::AppI {

public:
	MyApp(const char* _name, const char* _description, const char* _url);

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override;

	virtual int shutdown() override;

	bool update() override;

	// 更新 UI
	void updateImgui();

	// 更新输入
	void updateInput();

	// 更新灯光
	void updateLight();

private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
	int64_t m_timeOffset;

	Camera m_camera;				// 相机
	DirectionalLight m_light;		// 光源
	LightProbe m_lightProbe;		// 环境光 Probe

	Aery::CubeMesh m_cubeMesh;		// 方块模型
	Aery::PBRMesh m_pbrMesh;		// PBR模型
	Aery::SkyboxMesh m_skyMesh;		// skybox模型

	Aery::BaseMaterial m_baseMaterial;				// 基础材质
	Aery::BlinnPhongMaterial m_blinnPhongMaterial;	// blinnphong材质
	Aery::PBRMaterial m_PBRMaterial;				// PBR 材质
	Aery::IBLMaterial m_IBLMaterial;				// IBL 材质
	Aery::SkyMaterial m_skyMaterial;				// skybox材质
	Aery::ShadowMaterial m_shadowMaterial;			// 阴影材质

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;

	// Textures
	bgfx::UniformHandle s_texColor;
	bgfx::UniformHandle s_texNormal;
	bgfx::UniformHandle s_texAorm;
	// IBL
	bgfx::UniformHandle s_texCube;
	bgfx::UniformHandle s_texCubeIrr;
	bgfx::UniformHandle s_texLUT;
	// ShadowMap
	bgfx::UniformHandle s_shadowmap;

	// Settings
	bgfx::UniformHandle u_eyePos;
	bgfx::UniformHandle u_lightDir;
	bgfx::UniformHandle u_lightRadiance;
	bgfx::UniformHandle u_lightMtx;			// shadow mtx

	bgfx::UniformHandle u_k;
	float m_k[4] = { 0.2f,0.6f,0.2f ,8.0f};

	bgfx::UniformHandle u_ambient;
	float m_ambient[4] = {0.6f ,0.2f,0.2f};

	bgfx::UniformHandle u_mtx;				// skybox mtx

	// 输入相关
	entry::MouseState m_mouseState;		
	float m_mousex = 0.0f;
	float m_mousey = 0.0f;

	bool m_skyboxOn = 0;
	bool m_shadowOn = 0;
	int32_t m_meshSelection = 0;
	int32_t m_materialSelection = 0;
};
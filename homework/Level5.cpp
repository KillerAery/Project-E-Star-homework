#include "Level5.h"
#include <iostream>
#include <bx/uint32_t.h>
#include <entry/input.h>
#include "imgui/imgui.h"

bgfx::VertexLayout Level5::PosTangentNormalTexcoordVertex::ms_layout = {};
bgfx::VertexLayout Level5::PosVertex::ms_layout = {};

Level5::Level5(const char* _name, const char* _description, const char* _url)
	: entry::AppI(_name, _description, _url)
{
	m_width = 0;
	m_height = 0;
	m_debug = BGFX_DEBUG_NONE;
	m_reset = BGFX_RESET_NONE;
}

void Level5::init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height)
{
	Args args(_argc, _argv);

	m_width = _width;
	m_height = _height;
	m_debug = BGFX_DEBUG_TEXT;
	m_reset = BGFX_RESET_VSYNC;

	bgfx::Init init;
	init.type = args.m_type;
	init.vendorId = args.m_pciId;
	init.resolution.width = m_width;
	init.resolution.height = m_height;
	init.resolution.reset = m_reset;
	bgfx::init(init);

	// Enable debug text.
	bgfx::setDebug(m_debug);

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);	
	bgfx::setViewClear(1
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);

	// Create vertex stream declaration.
	PosTangentNormalTexcoordVertex::init();
	PosVertex::init();

	// Create texture sampler uniforms.
	s_texColor =	bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	s_texNormal =	bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
	s_texAorm =		bgfx::createUniform("s_texAorm", bgfx::UniformType::Sampler);
	s_shadowmap = bgfx::createUniform("s_shadowmap", bgfx::UniformType::Sampler);

	u_eyePos = bgfx::createUniform("u_eyePos", bgfx::UniformType::Vec4);
	u_lightDir	=	bgfx::createUniform("u_lightDir", bgfx::UniformType::Vec4);
	u_lightRadiance = bgfx::createUniform("u_lightRadiance", bgfx::UniformType::Vec4);
	u_k = bgfx::createUniform("u_k", bgfx::UniformType::Vec4);
	u_ambient = bgfx::createUniform("u_ambient", bgfx::UniformType::Vec4);
	u_lightMVP = bgfx::createUniform("u_lightMVP", bgfx::UniformType::Mat4);
	u_depthScaleOffset = bgfx::createUniform("u_depthScaleOffset", bgfx::UniformType::Vec4);

	// Create program from shaders.
	m_programPBR = loadProgram("vs_cubes_PBR_withShadow", "fs_cubes_PBR_withShadow");
	m_programShadow = loadProgram("vs_cubes_shadow", "fs_cubes_shadow");

	// Load albedo texture.
	m_textureColor = loadTexture("../resource/pbr_stone/pbr_stone_base_color.dds");
	// Load normal texture
	m_textureNormal = loadTexture("../resource/pbr_stone/pbr_stone_normal.dds");
	// Load arom texture
	m_textureAorm = loadTexture("../resource/pbr_stone/pbr_stone_aorm.dds");

	m_mesh = meshLoad("../resource/pbr_stone/pbr_stone_mes.bin");

	m_timeOffset = bx::getHPCounter();

	imguiCreate();
}

int Level5::shutdown()
{
	imguiDestroy();

	// Cleanup.
	bgfx::destroy(m_textureColor);
	bgfx::destroy(s_texColor);
	bgfx::destroy(m_textureNormal);
	bgfx::destroy(s_texNormal);
	bgfx::destroy(m_textureAorm);
	bgfx::destroy(s_texAorm);
	bgfx::destroy(m_shadowmap);

	bgfx::destroy(u_eyePos);
	bgfx::destroy(u_lightDir);
	bgfx::destroy(u_lightRadiance);
	bgfx::destroy(u_k);
	bgfx::destroy(u_ambient);
	bgfx::destroy(u_lightMVP);
	bgfx::destroy(u_depthScaleOffset);

	bgfx::destroy(m_shadowmapFB);
	meshUnload(m_mesh);

	bgfx::destroy(m_programPBR);
	//bgfx::destroy(m_programShadow)

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}

bool Level5::update()
{
	if (entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))return false;

	float time = (float)((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));

	// ---------------------------------- UI Events
	//

	updateImgui();

	// ---------------------------------- Input Events
	// 

	updateInput();

	// ---------------------------------- Draw Events
	//
	updateLight();
	drawShadowmap();
	drawPBRStone();

	bgfx::frame();
	return true;
}

void Level5::updateImgui()
{
	auto charType = inputGetChar();
	uint8_t untype = -1;
	if (charType == nullptr) {
		charType = &untype;
	}
	imguiBeginFrame(m_mouseState.m_mx
		, m_mouseState.m_my
		, (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
		| (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
		| (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
		, m_mouseState.m_mz
		, uint16_t(m_width)
		, uint16_t(m_height)
		, *charType
	);

	showExampleDialog(this);

	ImGui::SetNextWindowPos(
		ImVec2(m_width - m_width / 4.0f - 10.0f, 10.0f)
		, ImGuiCond_FirstUseEver
	);
	ImGui::SetNextWindowSize(
		ImVec2(m_width / 4.0f, 0)
		, ImGuiCond_FirstUseEver
	);
	ImGui::Begin("Settings"
		, NULL
		, 0
	);
	ImGui::PushItemWidth(180.0f);
	ImGui::Text("property:");
	ImGui::Indent();
	ImGui::SliderFloat("light Pos x", &m_light.pos.x, -100.0f, 100.0f);
	ImGui::SliderFloat("light Pos y", &m_light.pos.y, -100.0f, 100.0f);
	ImGui::SliderFloat("light Pos z", &m_light.pos.z, -100.0f, 100.0f);
	ImGui::ColorWheel("light Radiance", m_light.radiance_colorWheel, 1.0f);
	ImGui::ColorWheel("ambient", m_ambient, 1.0f);
	ImGui::SliderFloat("k_diffuse", &m_k[0], 0.0f, 1.0f);
	ImGui::SliderFloat("k_specular", &m_k[1], 0.0f, 1.0f);
	ImGui::SliderFloat("k_ambient", &m_k[2], 0.0f, 1.0f);

	ImGui::End();

	imguiEndFrame();
}

void Level5::updateInput()
{
	// 如果按住左键，则拖拽旋转镜头
	if (ImGui::GetIO().MouseDown[0]) {
		m_mousex += ImGui::GetIO().MouseDelta.x;
		m_mousey += ImGui::GetIO().MouseDelta.y;
	}
	m_mousey = bx::clamp(m_mousey, -90.0f, 90.0f);
	// 滚轮缩放镜头
	float mouseScroll = 1.0f + 0.05f * m_mouseState.m_mz;
	// WASD平移镜头
	float keyForward = 0.0f;
	float keyLeft = 0.0f;
	if (inputGetKeyState(entry::Key::KeyW))keyForward += 1.0f;
	if (inputGetKeyState(entry::Key::KeyS))keyForward -= 1.0f;
	if (inputGetKeyState(entry::Key::KeyA))keyLeft += 1.0f;
	if (inputGetKeyState(entry::Key::KeyD))keyLeft -= 1.0f;

	// 相机
	m_camera.changeRadiusScale(bx::max(mouseScroll, 0.1f));		// 相机环绕目标距离
	m_camera.moveCamera(keyLeft * 0.01f, keyForward * 0.01f);	// 相机移动
	m_camera.rotateCamera(m_mousex * 0.01f, m_mousey * 0.01f);	// 相机旋转
	m_camera.updateView();										// 更新view变换
}

void Level5::updateLight()
{
	m_light.updateDirection();
	m_light.updateRadiance();
	m_light.updateView();
	m_camera.updateProj(true, m_width, m_height);	// 投影变换
	m_light.updateMVP();
}

void Level5::drawShadowmap()
{	
	// Create shadowmap
	bgfx::setViewRect(0, 0, 0, m_shadowmapSize, m_shadowmapSize);	
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff, 1.0f, 0
	);

	bgfx::TextureHandle fbtextures[] =
	{
		bgfx::createTexture2D(
			  m_shadowmapSize
			, m_shadowmapSize
			, false
			, 1
			, bgfx::TextureFormat::BGRA8
			, BGFX_TEXTURE_RT
			),		
		bgfx::createTexture2D(
			  m_shadowmapSize
			, m_shadowmapSize
			, false
			, 1
			, bgfx::TextureFormat::D16
			, BGFX_TEXTURE_RT_WRITE_ONLY
			),

	};
	m_shadowmap = fbtextures[0];
	m_shadowmapFB = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
	bgfx::setViewFrameBuffer(0, m_shadowmapFB);
	bgfx::setViewTransform(0, m_light.view, m_light.proj);

	// 设置 shadow depthScaleOffset
	float depthScaleOffset[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	if (bgfx::getCaps()->homogeneousDepth)
	{
		depthScaleOffset[0] = 0.5f;
		depthScaleOffset[1] = 0.5f;
	}
	bgfx::setUniform(u_depthScaleOffset, depthScaleOffset);

	// 设置渲染状态
	bgfx::setState(
		BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_CULL_CCW
		| BGFX_STATE_MSAA
	);
	// 提交渲染命令
	float modelMatrix[16];
	bx::mtxSRT(modelMatrix, 2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.80f, 0.0f);
	meshSubmit(m_mesh, 0, m_programShadow, modelMatrix);
}

void Level5::drawPBRStone()
{	
	bgfx::setViewRect(1, 0, 0, uint16_t(m_width), uint16_t(m_height));
	bgfx::setViewClear(1
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);

	// 灯光相关	
	bgfx::setUniform(u_lightDir, &m_light.direction);		// 设置 light direction
	bgfx::setUniform(u_lightRadiance, m_light.radiance);	// 设置 radiance
	// ambient
	bgfx::setUniform(u_ambient, m_ambient);					// 设置 ambient
	// 绑定纹理
	bgfx::setTexture(0, s_texColor, m_textureColor);
	bgfx::setTexture(1, s_texNormal, m_textureNormal);
	bgfx::setTexture(2, s_texAorm, m_textureAorm);
	bgfx::setTexture(3, s_shadowmap, m_shadowmap);

	float* view = m_camera.view;
	float* proj = m_camera.proj;
	bgfx::setViewTransform(1, view, proj);			// 设置 view、proj
	bgfx::setUniform(u_eyePos, &m_camera.pos);		// 设置 相机位置
	bgfx::setUniform(u_k, m_k);						// 设置 diffuse、specular、ambient 系数

	bgfx::setUniform(u_lightMVP, m_light.MVP);				// 设置 light mvp

	// 设置渲染状态
	bgfx::setState(
		BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_CULL_CW
		| BGFX_STATE_MSAA
	);
	// 提交渲染命令
	float modelMatrix[16];
	bx::mtxSRT(modelMatrix, 2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.80f, 0.0f);
	meshSubmit(m_mesh, 1, m_programPBR, modelMatrix);
}
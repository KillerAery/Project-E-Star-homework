#include "Level4.h"
#include <bx/uint32_t.h>
#include <iostream>
#include "bgfx_logo.h"
#include "imgui/imgui.h"
#include <entry/input.h>

bgfx::VertexLayout Level4::PosTangentNormalTexcoordVertex::ms_layout = {};
bgfx::VertexLayout Level4::PosTexcoordVertex::ms_layout = {};

Level4::Level4(const char* _name, const char* _description, const char* _url)
	: entry::AppI(_name, _description, _url)
{
	m_width = 0;
	m_height = 0;
	m_debug = BGFX_DEBUG_NONE;
	m_reset = BGFX_RESET_NONE;
}

void Level4::init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height)
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

	// Create vertex stream declaration.
	PosTangentNormalTexcoordVertex::init();
	PosTexcoordVertex::init();

	// Create texture sampler uniforms.
	s_texColor =	bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	s_texNormal =	bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
	s_texAorm =		bgfx::createUniform("s_texAorm", bgfx::UniformType::Sampler);
	s_texCube =		bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler);
	s_texCubeIrr =	bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler);
	s_texLUT =		bgfx::createUniform("s_texLUT", bgfx::UniformType::Sampler);

	u_eyePos = bgfx::createUniform("u_eyePos", bgfx::UniformType::Vec4);
	u_lightDir	=	bgfx::createUniform("u_lightDir", bgfx::UniformType::Vec4);
	u_lightRadiance = bgfx::createUniform("u_lightRadiance", bgfx::UniformType::Vec4);
	u_k = bgfx::createUniform("u_k", bgfx::UniformType::Vec4);
	u_mtx = bgfx::createUniform("u_mtx", bgfx::UniformType::Mat4);

	// Create program from shaders.
	m_programIBL = loadProgram("vs_cubes_IBL", "fs_cubes_IBL");
	m_programSkybox = loadProgram("vs_cubes_IBL_skybox", "fs_cubes_IBL_skybox");

	// Load albedo texture.
	m_textureColor = loadTexture("../resource/pbr_stone/pbr_stone_base_color.dds");
	// Load normal texture
	m_textureNormal = loadTexture("../resource/pbr_stone/pbr_stone_normal.dds");
	// Load arom texture
	m_textureAorm = loadTexture("../resource/pbr_stone/pbr_stone_aorm.dds");
	// Light Probe
	m_lightProbe.load("kyoto");
	// Load LUT texture
	m_texLUT = loadTexture("../resource/GGX_E_LUT.dds");

	m_mesh = meshLoad("../resource/pbr_stone/pbr_stone_mes.bin");

	m_timeOffset = bx::getHPCounter();

	imguiCreate();
}

int Level4::shutdown()
{
	imguiDestroy();

	// Cleanup.
	meshUnload(m_mesh);
	bgfx::destroy(m_textureColor);
	bgfx::destroy(s_texColor);
	bgfx::destroy(m_textureNormal);
	bgfx::destroy(s_texNormal);
	bgfx::destroy(m_textureAorm);
	bgfx::destroy(s_texAorm);
	bgfx::destroy(s_texCube);
	bgfx::destroy(s_texCubeIrr);
	bgfx::destroy(m_texLUT);
	bgfx::destroy(s_texLUT);

	bgfx::destroy(u_eyePos);
	bgfx::destroy(u_lightDir);
	bgfx::destroy(u_lightRadiance);
	bgfx::destroy(u_k);
	bgfx::destroy(u_mtx);

	bgfx::destroy(m_programIBL);
	bgfx::destroy(m_programSkybox);

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}

bool Level4::update()
{
	if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
	{
		float time = (float)((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));

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
		ImGui::Text("PBR property:");
		ImGui::Indent();
		ImGui::SliderFloat("lightDir x", &m_lightDir[0], -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir y", &m_lightDir[1], -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir z", &m_lightDir[2], -1.0f, 1.0f);
		ImGui::SliderFloat("k_diffuse", &m_k[0], 0.0f, 1.0f);
		ImGui::SliderFloat("k_specular", &m_k[1], 0.0f, 1.0f);
		ImGui::ColorWheel("directional light", m_lightRadiance, 1.0f);

		ImGui::End();

		imguiEndFrame();

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
		bgfx::setViewRect(1, 0, 0, uint16_t(m_width), uint16_t(m_height));

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::touch(0);


		// ---------------------------------- Input Events
		// 
		// 如果按住左键，则拖拽旋转镜头
		if (ImGui::GetIO().MouseDown[0]) {
			m_mousex += ImGui::GetIO().MouseDelta.x;
			m_mousey += ImGui::GetIO().MouseDelta.y;
		}
		//m_mousex = bx::clamp(m_mousex,-90.0f ,90.0f );
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

		// ---------------------------------- Draw Events
		//

		// 相机相关
		m_cameraOffset = bx::add(m_cameraOffset,
			bx::add(bx::mul(m_cameraRight, keyLeft * 0.01f),
				bx::mul(m_cameraForward, keyForward * 0.01f))
		);
		// 相机目标
		const bx::Vec3 at = m_cameraOffset;

		// 相机位置
		float cameraRadius = 35.0f;
		cameraRadius *= mouseScroll;
		float horizonRadius = bx::cos(m_mousey * 0.01f);
		bx::Vec3 eye = {
			cameraRadius* bx::sin(m_mousex * 0.01f)* horizonRadius,
			cameraRadius* bx::sin(m_mousey * 0.01f),
			cameraRadius* bx::cos(m_mousex * 0.01f)* horizonRadius
		};
		eye = bx::add(eye, m_cameraOffset);
		bgfx::setUniform(u_eyePos, &eye);

		// 相机方向
		m_cameraForward = bx::sub(at,eye);
		m_cameraRight = bx::cross(m_cameraForward, bx::Vec3{0,1,0});
		float view[16];
		float proj[16];

		bx::mtxIdentity(view);
		bx::mtxLookAt(view, eye, at);
		bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		// 灯光相关		
		bx::Vec3 lightDir = {m_lightDir[0],m_lightDir[1],m_lightDir[2]};
		lightDir = bx::normalize(lightDir);
		float lightDir2Set[4] = { lightDir.x,lightDir.y,lightDir.z,0.0f };
		bgfx::setUniform(u_lightDir, lightDir2Set);
		float lightRadiance2Set[4] = { m_lightRadiance[0]*2000,m_lightRadiance[1] * 2000,m_lightRadiance[2] * 2000,1.0f};
		bgfx::setUniform(u_lightRadiance, lightRadiance2Set);
		bgfx::setUniform(u_k, m_k);

		// Env rotation.
		m_envRotDest += 0.01f;
		m_envRotCurr = bx::lerp(m_envRotCurr, m_envRotDest, 0.1f);
		float mtxEnvRot[16];
		bx::mtxRotateY(mtxEnvRot, m_envRotCurr);
		//float mtxTranslate[16];
		//bx::mtxTranslate(mtxTranslate, -eye.x,-eye.y,-eye.z);
		//bx::mtxMul(m_mtx, view, mtxTranslate);
		//bx::mtxMul(m_mtx, view, mtxEnvRot); // Used for Skybox.
		bgfx::setUniform(u_mtx, mtxEnvRot);

		// Bind textures.
		bgfx::setTexture(0, s_texColor, m_textureColor);
		bgfx::setTexture(1, s_texNormal, m_textureNormal);
		bgfx::setTexture(2, s_texAorm, m_textureAorm);
		bgfx::setTexture(3, s_texCube, m_lightProbe.m_tex);
		bgfx::setTexture(4, s_texCubeIrr, m_lightProbe.m_texIrr);
		bgfx::setTexture(5, s_texLUT, m_texLUT);

		// Set render states.
		bgfx::setState(
			BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
		);

		// draw PBR_Stone(IBL shader)
		float modelMatrix[16];
		bx::mtxSRT(modelMatrix, 2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.80f, 0.0f);
		meshSubmit(m_mesh,0, m_programIBL, modelMatrix);

		// draw Skybox（skybox shader）
		bx::mtxIdentity(view);
		bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(1, view, proj);
		bgfx::setUniform(u_mtx, mtxEnvRot);
		bgfx::setTexture(0, s_texCube, m_lightProbe.m_tex);
		bgfx::setState(
			BGFX_STATE_WRITE_RGB
			| BGFX_STATE_DEPTH_TEST_EQUAL
		);
		screenSpaceQuad((float)m_width, (float)m_height, true);
		bgfx::submit(1, m_programSkybox);

		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();

		return true;
	}

	return false;
}

void Level4::screenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft, float _width, float _height)
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

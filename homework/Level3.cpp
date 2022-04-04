#include "Level3.h"
#include <bx/uint32_t.h>
#include "bgfx_logo.h"
#include "imgui/imgui.h"
#include <entry/input.h>

bgfx::VertexLayout Level3::PosNormalTexcoordVertex::ms_layout = {};

Level3::Level3(const char* _name, const char* _description, const char* _url)
	: entry::AppI(_name, _description, _url)
{
	m_width = 0;
	m_height = 0;
	m_debug = BGFX_DEBUG_NONE;
	m_reset = BGFX_RESET_NONE;
}

void Level3::init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height)
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
	PosNormalTexcoordVertex::init();

	// Create texture sampler uniforms.
	s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	u_k					= bgfx::createUniform("u_k", bgfx::UniformType::Vec4);
	u_lightPos			= bgfx::createUniform("u_lightPos", bgfx::UniformType::Vec4);
	u_ambientIntensity	= bgfx::createUniform("u_ambientIntensity", bgfx::UniformType::Vec4);
	u_lightIntensity	= bgfx::createUniform("u_lightIntensity", bgfx::UniformType::Vec4);

	// Create program from shaders.
	m_program = loadProgram("vs_cubes_blinnphong", "fs_cubes_blinnphong");

	// Load diffuse texture.
	m_textureColor = loadTexture("../resource/pbr_stone/pbr_stone_base_color.dds", BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP);


	m_mesh = meshLoad("../resource/pbr_stone/pbr_stone_mes.bin");

	m_timeOffset = bx::getHPCounter();

	imguiCreate();
}

int Level3::shutdown()
{
	imguiDestroy();

	// Cleanup.
	meshUnload(m_mesh);
	bgfx::destroy(m_textureColor);
	bgfx::destroy(s_texColor);
	bgfx::destroy(u_k);
	bgfx::destroy(u_lightPos);
	bgfx::destroy(u_ambientIntensity);
	bgfx::destroy(u_lightIntensity);
	bgfx::destroy(m_program);

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}

bool Level3::update()
{
	if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
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
		ImGui::Text("blinn-phong property:");
		ImGui::Indent();
		ImGui::SliderFloat("ambient k", &m_ka, 0.0f, 1.0f);
		ImGui::SliderFloat("diffuse k", &m_kd, 0.0f, 1.0f);
		ImGui::SliderFloat("specular k", &m_ks, 0.0f, 1.0f);
		ImGui::SliderFloat("specular pow", &m_p, 1.0f, 64.0f);
		ImGui::ColorWheel("ambient color", m_ambientIntensity, 10.0f);
		ImGui::ColorWheel("light color", m_lightIntensity,10.0f);

		ImGui::End();

		imguiEndFrame();

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

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

		// ---------------------------------- Logic Events
		//
		float time = (float)((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));

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
		// 相机方向
		eye = bx::add(eye, m_cameraOffset);
		m_cameraForward = bx::sub(at,eye);
		m_cameraRight = bx::cross(m_cameraForward, bx::Vec3{0,1,0});

		// Set view and projection matrix for view 0.
		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);
		
		// 系数相关
		float k[4] = {m_ka,m_kd,m_ks,m_p};
		bgfx::setUniform(u_k, k);

		// 灯光相关
		float ambientIntensity[4];
		float lightIntensity[4];
		// 加强光照颜色
		for (int i = 0; i < 4; ++i){
			ambientIntensity[i] = m_ambientIntensity[i] * 10.0f;
			lightIntensity[i] = m_lightIntensity[i] * 20.0f;
		}
		bgfx::setUniform(u_ambientIntensity,ambientIntensity);
		bgfx::setUniform(u_lightIntensity, lightIntensity);
		float lightPos[4] = { bx::sin(time) * 3.0f,5.0f,bx::cos(time) * 3.0f,0.0f };
		bgfx::setUniform(u_lightPos, lightPos);

		// Submit 1 cubes.
		float mtx[16];
		bx::mtxSRT(mtx, 7, 7, 7, 0.21f, 0.37f, 0, 0, 0, 0);

		// Set model matrix for rendering.
		bgfx::setTransform(mtx);

		// Bind textures.
		bgfx::setTexture(0, s_texColor, m_textureColor);

		// Set render states.
		uint64_t state = 0
			| BGFX_STATE_WRITE_R
			| BGFX_STATE_WRITE_G
			| BGFX_STATE_WRITE_B
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
			;
		bgfx::setState(state);

		// 提交渲染命令
		meshSubmit(m_mesh,0, m_program, NULL);
		// Submit primitive for rendering to view 0.
		// bgfx::submit(0, m_program);

		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();

		return true;
	}

	return false;
}
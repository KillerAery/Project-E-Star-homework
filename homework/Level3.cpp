#include "Level3.h"
#include <bx/uint32_t.h>
#include "bgfx_logo.h"
#include "imgui/imgui.h"
#include <entry/input.h>

bgfx::VertexLayout Level3::PosTBNTexcoord3Vertex::ms_layout = {};

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
	PosTBNTexcoord3Vertex::init();

	// Create texture sampler uniforms.
	s_texColor =	bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	s_texNormal =	bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
	s_texAorm =		bgfx::createUniform("s_texAorm", bgfx::UniformType::Sampler);
	u_lightDir	=	bgfx::createUniform("u_lightDir", bgfx::UniformType::Vec4);
	u_lightRadiance = bgfx::createUniform("u_lightRadiance", bgfx::UniformType::Vec4);
	u_ambient = bgfx::createUniform("u_ambient", bgfx::UniformType::Vec4);
	u_eyePos = bgfx::createUniform("u_eyePos", bgfx::UniformType::Vec4);

	// Create program from shaders.
	m_program = loadProgram("vs_cubes_PBR", "fs_cubes_PBR");

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

int Level3::shutdown()
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

	bgfx::destroy(u_lightDir);
	bgfx::destroy(u_lightRadiance);
	bgfx::destroy(u_ambient);
	bgfx::destroy(u_eyePos);

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
		ImGui::Text("PBR property:");
		ImGui::Indent();
		ImGui::SliderFloat("lightDir x", &m_lightDir[0], -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir y", &m_lightDir[1], -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir z", &m_lightDir[2], -1.0f, 1.0f);
		ImGui::ColorWheel("light radiance", m_lightRadiance, 1.0f);
		ImGui::ColorWheel("ambient", m_ambient, 1.0f);

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
		
		bgfx::setUniform(u_eyePos, &eye);

		// 灯光相关		
		bx::Vec3 lightDir = {m_lightDir[0],m_lightDir[1],m_lightDir[2]};
		lightDir = bx::normalize(lightDir);
		float lightDir2Set[4] = { lightDir.x,lightDir.y,lightDir.z,0.0f };
		bgfx::setUniform(u_lightDir, lightDir2Set);
		float lightRadiance2Set[4] = { m_lightRadiance[0]*200000,m_lightRadiance[1] * 200000,m_lightRadiance[2] * 200000,1.0f};
		bgfx::setUniform(u_lightRadiance, lightRadiance2Set);
		bgfx::setUniform(u_ambient, m_ambient);

		// Submit 1 cubes.
		float mtx[16];
		bx::mtxSRT(mtx, 7, 7, 7, 0.21f, 0.37f, 0, 0, 0, 0);

		// Set model matrix for rendering.
		bgfx::setTransform(mtx);

		// Bind textures.
		bgfx::setTexture(0, s_texColor, m_textureColor);
		bgfx::setTexture(1, s_texNormal, m_textureNormal);
		bgfx::setTexture(2, s_texAorm, m_textureAorm);

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
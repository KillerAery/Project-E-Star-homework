#include "MyApp.h"
#include <bx/uint32_t.h>
#include <entry/input.h>
#include "imgui/imgui.h"
#include "InputLayout.h"

MyApp::MyApp(const char* _name, const char* _description, const char* _url)
	: entry::AppI(_name, _description, _url)
{
	m_width = 0;
	m_height = 0;
	m_debug = BGFX_DEBUG_NONE;
	m_reset = BGFX_RESET_NONE;
}

void MyApp::init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height)
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
	PosColorVertex::init();
	PosNormalTexcoordVertex::init();
	PosTangentNormalTexcoordVertex::init();
	PosTexcoordVertex::init();

	// Create texture sampler uniforms.
	s_texColor		= bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	s_texNormal		= bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
	s_texAorm		= bgfx::createUniform("s_texAorm", bgfx::UniformType::Sampler);
	s_texCube		= bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler);
	s_texCubeIrr	= bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler);
	s_texLUT		= bgfx::createUniform("s_texLUT", bgfx::UniformType::Sampler);
	s_shadowmap		= bgfx::createUniform("s_shadowmap", bgfx::UniformType::Sampler);

	u_eyePos		= bgfx::createUniform("u_eyePos", bgfx::UniformType::Vec4);
	u_lightDir		= bgfx::createUniform("u_lightDir", bgfx::UniformType::Vec4);
	u_lightRadiance = bgfx::createUniform("u_lightRadiance", bgfx::UniformType::Vec4);
	u_lightMtx		= bgfx::createUniform("u_lightMtx", bgfx::UniformType::Mat4);
	u_k				= bgfx::createUniform("u_k", bgfx::UniformType::Vec4);
	u_ambient		= bgfx::createUniform("u_ambient", bgfx::UniformType::Vec4);
	u_mtx			= bgfx::createUniform("u_mtx", bgfx::UniformType::Mat4);


	// cube 模型加载
	m_cubeMesh.AddStaticMesh(
		m_vbh, m_ibh
	);

	// PBR 模型加载
	m_pbrMesh.LoadMesh(
		"../resource/pbr_stone/pbr_stone_mes.bin",
		"../resource/pbr_stone/pbr_stone_base_color.dds",
		"../resource/pbr_stone/pbr_stone_normal.dds",
		"../resource/pbr_stone/pbr_stone_aorm.dds"
	);

	// LightProbe 加载
	m_lightProbe.load("kyoto");

	m_baseMaterial.init();
	m_blinnPhongMaterial.init();
	m_PBRMaterial.init();
	m_IBLMaterial.init();
	m_skyMaterial.init();
	m_shadowMaterial.init();

	m_timeOffset = bx::getHPCounter();
	imguiCreate();
}

int MyApp::shutdown()
{
	imguiDestroy();

	// Cleanup.
	bgfx::destroy(s_texColor);
	bgfx::destroy(s_texNormal);
	bgfx::destroy(s_texAorm);
	bgfx::destroy(s_texCube);
	bgfx::destroy(s_texCubeIrr);
	bgfx::destroy(s_texLUT);
	bgfx::destroy(s_shadowmap);

	bgfx::destroy(u_eyePos);
	bgfx::destroy(u_lightDir);
	bgfx::destroy(u_lightRadiance);
	bgfx::destroy(u_lightMtx);

	bgfx::destroy(u_k);
	bgfx::destroy(u_ambient);
	bgfx::destroy(u_mtx);

	bgfx::destroy(m_vbh);
	bgfx::destroy(m_ibh);

	m_baseMaterial.destory();
	m_blinnPhongMaterial.destory();
	m_PBRMaterial.destory();
	m_IBLMaterial.destory();
	m_skyMaterial.destory();
	m_shadowMaterial.destory();

	m_cubeMesh.destory();
	m_pbrMesh.destory();
	m_skyMesh.destory();

	m_lightProbe.destroy();

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}

bool MyApp::update()
{
	if (entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))return false;

	float time = (float)((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));

	// ---------------------------------- UI Events
	//

	updateImgui();

	// ---------------------------------- Input Events
	// 

	updateInput();

	// ---------------------------------- Light Events
	//

	updateLight();

	// ---------------------------------- Draw Events
	//

	bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
	bgfx::setViewRect(1, 0, 0, m_shadowMaterial.getShadowmapSize(), m_shadowMaterial.getShadowmapSize());
	bgfx::setViewFrameBuffer(1, m_shadowMaterial.getShadowmapFrameBuffer());
	bgfx::setViewRect(2, 0, 0, uint16_t(m_width), uint16_t(m_height));


	float modelMatrix[3][16];
	bx::mtxSRT(modelMatrix[0], 2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	bx::mtxSRT(modelMatrix[1], 10.0f, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, -70.0f, 0.0f);
	bx::mtxSRT(modelMatrix[2], 10.0f, 10.0f, 10.0f, 45.0f, 45.0f, 45.0f, 0.0f, 0.0f, 0.0f);

	// Shadow
	m_PBRMaterial.setShadowOn(m_shadowOn);
	m_blinnPhongMaterial.setShadowOn(m_shadowOn);
	m_IBLMaterial.setShadowOn(m_shadowOn);
	if (m_shadowOn) {
		bgfx::setViewTransform(1, m_light.view, m_light.proj);
		m_shadowMaterial.setState();
		m_shadowMaterial.drawMesh(m_pbrMesh, modelMatrix[0]);
		m_shadowMaterial.drawMesh(m_pbrMesh, modelMatrix[1]);
	}

	// Main
	bgfx::setTexture(0, s_texColor, m_pbrMesh.getTexColor());
	bgfx::setTexture(1, s_texNormal, m_pbrMesh.getTexNormal());
	bgfx::setTexture(2, s_texAorm, m_pbrMesh.getTexAorm());
	bgfx::setTexture(3, s_texCube, m_lightProbe.getTexLOD());
	//bgfx::setTexture(4, s_texCubeIrr, m_lightProbe.getTexIrr());
	bgfx::setTexture(5, s_texLUT, m_IBLMaterial.getTexLUT());
	bgfx::setTexture(6, s_shadowmap, m_shadowMaterial.getShadowmap());

	bgfx::setViewTransform(0, m_camera.view, m_camera.proj);// 设置 view、proj
	bgfx::setUniform(u_mtx, m_camera.view_inv);

	bgfx::setUniform(u_eyePos, &m_camera.pos);				// 设置 相机位置

	bgfx::setUniform(u_lightDir, &m_light.direction);		// 设置 light direction
	bgfx::setUniform(u_lightRadiance, m_light.radiance);	// 设置 light radiance
	bgfx::setUniform(u_lightMtx, m_light.lightMtx);			// 设置 light mtx

	bgfx::setUniform(u_k, m_k);								// 设置 diffuse、specular、ambient 系数
	bgfx::setUniform(u_ambient, m_ambient);					// 设置 ambient

	if (m_meshSelection == 0) {
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::setIndexBuffer(m_ibh);
		m_baseMaterial.setState();
		m_baseMaterial.drawMesh(m_cubeMesh, modelMatrix[2]);
	}
	else if (m_meshSelection == 1) {
		if (m_materialSelection == 1) {
			m_blinnPhongMaterial.setState();
			m_blinnPhongMaterial.drawMesh(m_pbrMesh, modelMatrix[0]);
		}
		else if (m_materialSelection == 2) {
			m_PBRMaterial.setState();
			m_PBRMaterial.drawMesh(m_pbrMesh, modelMatrix[0]);
		}
		else if(m_materialSelection == 3) {
			m_IBLMaterial.setState();
			m_IBLMaterial.drawMesh(m_pbrMesh, modelMatrix[0]);
		}
	}

	// Skybox
	if (m_skyboxOn) {
		bgfx::setTexture(0, s_texCube, m_lightProbe.getTexLOD());
		float view_identity[16];
		bx::mtxIdentity(view_identity);

		m_camera.updateProj(false);			// 正交投影
		bgfx::setViewTransform(2, view_identity, m_camera.proj);

		bgfx::setUniform(u_mtx, m_camera.view_inv);

		m_skyMesh.AddMesh((float)m_width, (float)m_height, true);
		m_skyMaterial.setState();
		m_skyMaterial.drawMesh(m_skyMesh);
	}

	bgfx::frame();
	return true;
}

void MyApp::updateImgui()
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
		ImVec2(m_width / 4.0f, m_height *3.0f/4)
		, ImGuiCond_FirstUseEver
	);
	ImGui::Begin("Settings"
		, NULL
		, 0
	);
	ImGui::PushItemWidth(180.0f);
	ImGui::Text("property:");

	ImGui::Checkbox("Shadow", &m_shadowOn);
	ImGui::Checkbox("Skybox", &m_skyboxOn);

	ImGui::Text("Mesh:");
	ImGui::Indent();
	ImGui::RadioButton("Cubes", &m_meshSelection, 0);
	ImGui::RadioButton("Stones", &m_meshSelection, 1);
	ImGui::Unindent();
	// Cubes
	if (m_meshSelection == 0) {
		m_materialSelection = 0;
		ImGui::Text("Material:");
		ImGui::Indent();
		ImGui::RadioButton("Base", &m_materialSelection, 0);
		ImGui::Unindent();
	}
	else if (m_meshSelection == 1) {
		if (m_materialSelection == 0) { 
			m_materialSelection = 1; 
		}
		ImGui::Text("Material:");
		ImGui::Indent();
		ImGui::RadioButton("BlinnPhong",	&m_materialSelection, 1);
		ImGui::RadioButton("PBR",			&m_materialSelection, 2);
		ImGui::RadioButton("IBL",			&m_materialSelection, 3);
		ImGui::Unindent();
	}


	ImGui::Indent();
	if (m_materialSelection == 0) {

	}
	else if (m_materialSelection == 1) {
		ImGui::SliderFloat("lightDir x", &m_light.direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir y", &m_light.direction.y, -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir z", &m_light.direction.z, -1.0f, 1.0f);
		ImGui::ColorWheel("light intensity", m_light.radiance_colorWheel, 1.0f);
		ImGui::SliderFloat("k_diffuse", &m_k[0], 0.0f, 1.0f);
		ImGui::SliderFloat("k_specular", &m_k[1], 0.0f, 1.0f);
		ImGui::SliderFloat("k_ambient", &m_k[2], 0.0f, 1.0f);
		ImGui::SliderFloat("specular pow", &m_k[3], 1.0f, 64.0f);
		ImGui::ColorWheel("ambient", m_ambient, 1.0f);
	}
	else if (m_materialSelection == 2) {
		ImGui::SliderFloat("lightDir x", &m_light.direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir y", &m_light.direction.y, -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir z", &m_light.direction.z, -1.0f, 1.0f);
		ImGui::ColorWheel("light radiance", m_light.radiance_colorWheel, 1.0f);
		ImGui::SliderFloat("k_diffuse", &m_k[0], 0.0f, 1.0f);
		ImGui::SliderFloat("k_specular", &m_k[1], 0.0f, 1.0f);
		ImGui::SliderFloat("k_ambient", &m_k[2], 0.0f, 1.0f);
		ImGui::ColorWheel("ambient", m_ambient, 1.0f);
	}
	else if (m_materialSelection == 3) {
		ImGui::SliderFloat("lightDir x", &m_light.direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir y", &m_light.direction.y, -1.0f, 1.0f);
		ImGui::SliderFloat("lightDir z", &m_light.direction.z, -1.0f, 1.0f);
		ImGui::ColorWheel("light radiance", m_light.radiance_colorWheel, 1.0f);
		ImGui::SliderFloat("k_diffuse", &m_k[0], 0.0f, 1.0f);
		ImGui::SliderFloat("k_specular", &m_k[1], 0.0f, 1.0f);
	}
	ImGui::Unindent();


	ImGui::End();

	imguiEndFrame();
}

void MyApp::updateInput()
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
	m_camera.updateProj(true, m_width, m_height);				// 投影变换
}


void MyApp::updateLight()
{
	m_light.updateDirection();
	m_light.updateRadiance(10);
	m_light.updateProj();
	m_light.updateView();
	m_light.updateLightMtx();
}
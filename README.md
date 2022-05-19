# Project-E-Star Homework

based on [bgfx framework](https://github.com/bkaradzic/bgfx)

# Display

## Level1

Features：

1. 加载模型，绘制在屏幕上
2. 添加环绕相机（Orbit Camera），并可以使用鼠标操控：
   - 鼠标左键拖拽以旋转镜头(类似geometryv里的操作方式)
   - 鼠标滚轮缩放镜头(类似geometryv里的操作方式)

Base Material：

<img src="https://imagebed-aery.oss-cn-hangzhou.aliyuncs.com/202205192226389.png" alt="image-20220504182404039" style="zoom: 67%;" />

## Level2

Features：

1. 为模型添加基础纹理
2. 为模型添加基础光照（Blinn-Phong）
3. 在保留环绕相机功能的情况下，为相机添加键盘控制:
   - ADWS控制镜头左右上下平移

BlinnPhong Material：

<img src="https://imagebed-aery.oss-cn-hangzhou.aliyuncs.com/202205192226390.png" alt="image-20220504182742561" style="zoom:67%;" />

## Level3

Features：

1. 把模型的直接光漫反射光照改为PBR模型实现
   - 模型的金属度能正确影响漫反射光照
   - 模型的albedo (反照率)使用纹理控制
2. 把模型的直接光高光改为PBR模型实现
   - 模型的金属度，粗糙度通过纹理控制

PBR Material：

<img src="https://imagebed-aery.oss-cn-hangzhou.aliyuncs.com/202205192226391.png" alt="image-20220504182722869" style="zoom: 67%;" />

## Level4

Features：

1. 使用 IBL（Image-Based Lighting），为模型添加环境光照的漫反射部分
2. 使用 lBL（Image-Based Lighting），为模型添加环境光照的高光反射部分
3. 添加一个包住场景的天空盒
4. 天空盒使用的cubemap纹理，对应IBL图的mipmap level 0

IBL Material（split sum 方法）：

![homework_K27ygIAeK5](https://imagebed-aery.oss-cn-hangzhou.aliyuncs.com/202205192226392.jpg)

## Level5

Features：

1. 使用 ShadowMap 的方式，为模型添加阴影

BlinnPhong with ShadowMap：

<img src="https://imagebed-aery.oss-cn-hangzhou.aliyuncs.com/202205192226393.png" alt="image-20220504182928586" style="zoom:67%;" />

PBR with ShadowMap：

<img src="https://imagebed-aery.oss-cn-hangzhou.aliyuncs.com/202205192226394.png" alt="image-20220504182911546" style="zoom:67%;" />

IBL（split sum）with ShadowMap：

![homework_JVER2cehsN](https://imagebed-aery.oss-cn-hangzhou.aliyuncs.com/202205192226395.jpg)

# Framework

![image-20220504182109124](https://imagebed-aery.oss-cn-hangzhou.aliyuncs.com/202205192226396.png)

## MyApp

主要驱动应用，负责调度其它类型的对象来进行渲染

## DirectionalLight

方向光源，提供方向光渲染参数

## Camera

摄像机，提供视角相关渲染参数

## InputLayout

用于定义各种 shader 的顶点输入布局

## LightProbe

提供环境光的 LOD texture 和 Irradiance texture

## IMesh（模型接口）

| 子类       | 描述                                                         |
| ---------- | ------------------------------------------------------------ |
| CubeMesh   | 程序化生成的八个 vertex 组成的 Cube 形状的 Mesh              |
| PBRMesh    | 含有 albedo texture、normal texture、aorm texture(ambient occulsion, roughness, metallic) 三张贴图的 Mesh |
| SkyboxMesh | 程序化生成的覆盖屏幕空间的三角形 Mesh，专用于 Skybox Material 的渲染 |

## IMaterial（材质接口）

可调用 IMesh 对象并进行相应材质的渲染

| 子类               | 描述                                                         |
| ------------------ | ------------------------------------------------------------ |
| BaseMaterial       | 基础渲染                                                     |
| BlinnPhongMaterial | BlinnPhong 渲染（含 shadow 开关效果）                        |
| PBRMaterial        | PBR渲染（含 shadow 开关效果）：Lambert Diffuse BRDF + Cook-Torrance Specular BRDF |
| IBLMaterial        | PBR渲染（含 shadow 开关效果）：Lambert Diffuse BRDF + Cook-Torrance Specular BRDF；IBL 间接光（环境光）：使用 split sum 方法，即预过滤环境贴图 + BRDF积分LUT |
| SkyMaterial        | Skybox 渲染                                                  |
| ShadowMaterial     | 渲染到 texture ，用于产生 ShadowMap                          |

# Shaders

> 由于 bgfx 采用的是 sc 后缀格式的 shader，而且没有相应的语法高亮插件，因此这里我改成 glsl 后缀来编写实际上是 sc 的 shader

| Shader                        | 描述                                       |
| ----------------------------- | ------------------------------------------ |
| vs_cube.glsl                  | 基础渲染 vertex shader                     |
| fs_cube.glsl                  | 基础渲染 fragment shader                   |
| vs_blinnphong.glsl            | BlinnPhong vertex shader                   |
| fs_blinnphong.glsl            | BlinnPhong fragment shader                 |
| vs_blinnphong_withShadow.glsl | BlinnPhong vertex shader，带 shadow 效果   |
| fs_blinnphong_withShadow.glsl | BlinnPhong fragment shader，带 shadow 效果 |
| vs_PBR.glsl                   | PBR vertex shader                          |
| fs_PBR.glsl                   | PBR fragment shader                        |
| vs_PBR_withShadow.glsl        | PBR vertex shader                          |
| fs_PBR_withShadow.glsl        | PBR fragment shader                        |
| vs_IBL.glsl                   | IBL vertex shader                          |
| fs_IBL.glsl                   | IBL fragment shader                        |
| vs_IBL_withShadow.glsl        | IBL vertex shader，带shadow效果            |
| fs_IBL_withShadow.glsl        | IBL fragment shader，带shadow效果          |
| vs_skybox.glsl                | skybox vertex shader                       |
| fs_skybox.glsl                | skybox fragment shader                     |
| vs_shadow.glsl                | shadow vertex shader                       |
| fs_shadow.glsl                | shadow fragment shader                     |
| shadow.glsl                   | shadow 函数库，用于提供 shadow 效果        |





### 


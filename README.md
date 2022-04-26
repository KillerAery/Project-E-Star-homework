# project-e-star-homework

# Level1

1. 加载模型，绘制在屏幕上。
2. 添加环绕相机（Orbit Camera），并可以使用鼠标操控：
   - 鼠标左键拖拽以旋转镜头(类似geometryv里的操作方式)
   - 鼠标滚轮缩放镜头(类似geometryv里的操作方式)

# Level2

1. 为模型添加基础纹理
2. 为模型添加基础光照（Blinn-Phong）
3. 在保留环绕相机功能的情况下，为相机添加键盘控制:
   - ADWS控制镜头左右上下平移

# Level3

1. 把模型的直接光漫反射光照改为PBR模型实现
   - 模型的金属度能正确影响漫反射光照
   - 模型的albedo (反照率)使用纹理控制
2. 把模型的直接光高光改为PBR模型实现
   - 模型的金属度，粗糙度通过纹理控制

# Level4

1. 使用 IBL（Image-Based Lighting），为模型添加环境光照的漫反射部分
2. 使用 lBL（Image-Based Lighting），为模型添加环境光照的高光反射部分
3. 添加一个包住场景的天空盒
4. 天空盒使用的cubemap纹理，对应IBL图的mipmap level 0

# Level5

1. 使用 ShadowMap 的方式，为模型添加阴影
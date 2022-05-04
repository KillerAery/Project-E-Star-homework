#pragma once
#include "InputLayout.h"
#include "IMesh.h"

namespace Aery {
	class SkyboxMesh : public IMesh {
	public:
		SkyboxMesh();

		virtual ~SkyboxMesh();

		// Ìí¼Ó Mesh µ½¶¥µã»º´æ
		void AddMesh(
			float _textureWidth, 
			float _textureHeight, 
			bool _originBottomLeft = false,
			float _width = 1.0f,
			float _height = 1.0f
		);

		void destory() override;
	};
}
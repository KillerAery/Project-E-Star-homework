#pragma once
#include "InputLayout.h"
#include "IMesh.h"

namespace Aery {
	class PBRMesh : public IMesh {
	public:
		PBRMesh();
		virtual ~PBRMesh();

		// ╪сть Mesh
		void LoadMesh(
			const char* meshName,
			const char* baseColorMapName,
			const char* normalMapName,
			const char* aromMapName
			);

		void destory() override;

		bgfx::TextureHandle getTexColor();
		
		bgfx::TextureHandle getTexNormal();

		bgfx::TextureHandle getTexAorm();

	private:
		bgfx::TextureHandle m_textureColor;		// BaseColor Texture
		bgfx::TextureHandle m_textureNormal;	// Normal Texture
		bgfx::TextureHandle m_textureAorm;		// Aorm Texture
	};
}

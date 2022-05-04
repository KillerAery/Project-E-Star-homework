#include "InputLayout.h"

namespace Aery {
	bgfx::VertexLayout PosTexcoordVertex::ms_layout = {};
	bgfx::VertexLayout PosTangentNormalTexcoordVertex::ms_layout = {};
	bgfx::VertexLayout PosNormalTexcoordVertex::ms_layout = {};
	bgfx::VertexLayout PosColorVertex::ms_layout = {};
}
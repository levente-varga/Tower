#include "Box.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsMacros.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Topology.h"
#include "TransformConstantBuffer.h"

Box::Box(
	Graphics& graphics,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist
) :
	r(rdist(rng)),
	dRoll(ddist(rng)),
	dPitch(ddist(rng)),
	dYaw(ddist(rng)),
	dPhi(odist(rng)),
	dTheta(odist(rng)),
	dChi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		}
		position;

		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		}
		color;
	};

	const std::vector<Vertex> vertices =
	{
		{ -1.0f, -1.0f, -1.0f, 0,   0,   0,   255},
		{  1.0f, -1.0f, -1.0f, 255, 0,   0,   255},
		{ -1.0f,  1.0f, -1.0f, 0,   255, 0,   255},
		{  1.0f,  1.0f, -1.0f, 255, 255, 0,   255},
		{ -1.0f, -1.0f,  1.0f, 0,   0,   255, 255},
		{  1.0f, -1.0f,  1.0f, 255, 0,   255, 255},
		{ -1.0f,  1.0f,  1.0f, 0,   255, 255, 255},
		{  1.0f,  1.0f,  1.0f, 255, 255, 255, 255},
	};

	AddBind(std::make_unique<VertexBuffer>(graphics, vertices));

	auto pVertexShader = std::make_unique<VertexShader>(graphics, L"VertexShader.cso");
	auto pVertexShaderByteCode = pVertexShader->GetBytecode();

	AddBind(std::move(pVertexShader));

	AddBind(std::make_unique<PixelShader>(graphics, L"PixelShader.cso"));

	const std::vector<unsigned short> indices =
	{
		1, 0, 2,   3, 1, 2,
		0, 1, 4,   1, 5, 4,
		2, 0, 4,   6, 2, 4,
		4, 5, 6,   5, 7, 6,
		3, 2, 6,   7, 3, 6,
		1, 3, 5,   3, 7, 5,
	};

	AddIndexBuffer(std::make_unique<IndexBuffer>(graphics, indices));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
	{
		{
			"Position",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0,
		},
		{
			"Color",
			0,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			0,
			12, // 12 bytes offset
			D3D11_INPUT_PER_VERTEX_DATA,
			0,
		},
	};
	AddBind(std::make_unique<InputLayout>(graphics, inputElementDesc, pVertexShaderByteCode));
	AddBind(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	OutputDebugStringA(this == NULL ? "NULL" : "OK");
	OutputDebugStringA(" - in Box Constructor\n");
	AddBind(std::make_unique<TransformConstantBuffer>(graphics, *this));
}

void Box::Update(float delta) noexcept {
	roll += dRoll * delta;
	pitch += dPitch * delta;
	yaw += dYaw * delta;
	theta += dTheta * delta;
	phi += dPhi * delta;
	chi += dChi * delta;
}

DirectX::XMMATRIX Box::GetTransformMatrix() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
		* DirectX::XMMatrixTranslation(r, 0, 0)
		* DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi)
		* DirectX::XMMatrixTranslation(0, 0, 20);
}
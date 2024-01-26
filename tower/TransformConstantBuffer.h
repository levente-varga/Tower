#pragma once

#include "ConstantBuffer.h"
#include "Bindable.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& graphics, const Drawable& parent);
	~TransformConstantBuffer() noexcept = default;
	void Bind(Graphics& graphics) noexcept override;

private:
	VertexConstantBuffer<DirectX::XMMATRIX> vertexConstantBuffer;
	const Drawable& parent;
};
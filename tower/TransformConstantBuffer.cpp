#include "TransformConstantBuffer.h"
#include <iostream>

TransformConstantBuffer::TransformConstantBuffer(Graphics& graphics, const Drawable& drawable)
	: vertexConstantBuffer(graphics), parent(drawable)
{
	OutputDebugStringA(&parent == NULL ? "NULL" : "OK");
	OutputDebugStringA(" - in TransformConstantBuffer Constructor\n");
}

void TransformConstantBuffer::Bind(Graphics& graphics) noexcept
{
	vertexConstantBuffer.Update(
		graphics,
		DirectX::XMMatrixTranspose(
			parent.GetTransformMatrix() * graphics.GetProjectionMatrix()
		)
	);

	vertexConstantBuffer.Bind(graphics);
}
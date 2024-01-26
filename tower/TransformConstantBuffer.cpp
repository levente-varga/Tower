#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& graphics, const Drawable& drawable)
	: vertexConstantBuffer(graphics), parent(parent)
{
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
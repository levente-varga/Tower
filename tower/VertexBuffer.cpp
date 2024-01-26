#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& graphics) noexcept
{
	const UINT offset = 0u;
	GetContext(graphics)->IASetVertexBuffers(
		0u,
		1u,
		pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);
}
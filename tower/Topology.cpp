#include "Topology.h"

Topology::Topology(Graphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	type(type)
{
}

void Topology::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->IASetPrimitiveTopology(type);
}
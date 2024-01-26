#include "IndexBuffer.h"
#include "GraphicsMacros.h"

IndexBuffer::IndexBuffer(Graphics& graphics, const std::vector<unsigned short>& indices)
	:
	count((UINT)indices.size())
{
	INFO_MANAGER(graphics);

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDescription.Usage = D3D11_USAGE_DEFAULT;
	bufferDescription.CPUAccessFlags = 0u;
	bufferDescription.MiscFlags = 0u;
	bufferDescription.ByteWidth = UINT(count * sizeof(unsigned short));
	bufferDescription.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = indices.data();

	GRAPHICS_THROW_INFO(GetDevice(graphics)->CreateBuffer(&bufferDescription, &subresourceData, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
#pragma once

#include "Bindable.h"
#include "GraphicsMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<class T>
	VertexBuffer(Graphics& graphics, const std::vector<T>& vertices)
		:
		stride(sizeof(T))
	{
		INFO_MANAGER(graphics);

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = UINT(sizeof(T) * vertices.size());
		bufferDesc.StructureByteStride = sizeof(T);
		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = vertices.data();
		GRAPHICS_THROW_INFO(GetDevice(graphics)->CreateBuffer(
			&bufferDesc,
			&subresourceData,
			&pVertexBuffer
		));
	}
	void Bind(Graphics& graphics) noexcept override;

protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
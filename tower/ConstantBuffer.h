#pragma once

#include "Bindable.h"
#include <d3d11.h>
#include <wrl.h>
#include "GraphicsMacros.h"

template <typename T>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics& graphics, const T& constants)
	{
		INFO_MANAGER(graphics);

		D3D11_BUFFER_DESC bufferDescription;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDescription.MiscFlags = 0;
		bufferDescription.ByteWidth = sizeof(constants);
		bufferDescription.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = &constants;

		GRAPHICS_THROW_INFO(GetDevice(graphics)->CreateBuffer(
			&bufferDescription,
			&subresourceData,
			&pConstantBuffer
		));
	}
	ConstantBuffer(Graphics& graphics)
	{
		INFO_MANAGER(graphics);

		D3D11_BUFFER_DESC bufferDescription;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDescription.MiscFlags = 0;
		bufferDescription.ByteWidth = sizeof(T);
		bufferDescription.StructureByteStride = 0;

		GRAPHICS_THROW_INFO(GetDevice(graphics)->CreateBuffer(
			&bufferDescription,
			nullptr,
			&pConstantBuffer
		));
	}
	~ConstantBuffer() noexcept = default;
	void Update(Graphics& graphics, const T& constants) noexcept
	{
		INFO_MANAGER(graphics);

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		GRAPHICS_THROW_INFO(GetContext(graphics)->Map(
			pConstantBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedSubresource
		));

		memcpy(mappedSubresource.pData, &constants, sizeof(constants));
		GetContext(graphics)->Unmap(pConstantBuffer.Get(), 0);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->VSSetConstantBuffers(
			0,
			1,
			pConstantBuffer.GetAddressOf()
		);
	}
};

template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->PSSetConstantBuffers(
			0,
			1,
			pConstantBuffer.GetAddressOf()
		);
	}
};
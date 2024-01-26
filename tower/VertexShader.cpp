#include "VertexShader.h"
#include "GraphicsMacros.h"

VertexShader::VertexShader(Graphics& graphics, const std::wstring& path)
{
	INFO_MANAGER(graphics);

	GRAPHICS_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));
	GRAPHICS_THROW_INFO(GetDevice(graphics)->CreateVertexShader(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	));
}

void VertexShader::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->VSSetShader(
		pVertexShader.Get(),
		nullptr,
		0u
	);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pBytecodeBlob.Get();
}
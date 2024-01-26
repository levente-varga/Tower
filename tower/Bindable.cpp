#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& graphics) noexcept
{
	return graphics.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& graphics) noexcept
{
	return graphics.pDevice.Get();
}

DebugInfoManager& Bindable::GetInfoManager(Graphics& graphics) noexcept(!IS_DEBUG)
{
#ifndef NDEBUG
	return graphics.infoManager;
#else
	// InfoManager is not available in Release config
	throw std::logic_error("Tried to access graphics.infoManager in Release config");
#endif
}
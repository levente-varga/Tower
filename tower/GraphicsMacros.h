#pragma once

/*
 * [!] HRESULT hResult must exist in the local scope for these macros to work
 */

#define GRAPHICS_EXCEPTION_NO_INFO(hResult) Graphics::GraphicsException(__LINE__, __FILE__, (hr))
#define GRAPHICS_THROW_NO_INFO(hResultCall) if (FAILED(hResult = (hResultCall))) throw Graphics::GraphicsException(__LINE__, __FILE__, hResult);

#ifndef NDEBUG

#define GRAPHICS_EXCEPTION(hResult) Graphics::GraphicsException(__LINE__, __FILE__, (hResult), infoManager.GetMessages())
#define GRAPHICS_THROW_INFO(hResultCall) infoManager.Set(); if (FAILED(hResult = (hResultCall))) throw GRAPHICS_EXCEPTION(hResult)
#define GRAPHICS_DEVICE_REMOVED_EXCEPTION(hResult) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hResult), infoManager.GetMessages())
#define GRAPHICS_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto messages = infoManager.GetMessages(); if (!messages.empty()) {throw Graphics::InfoException(__LINE__, __FILE__, messages);}}

#else

#define GRAPHICS_EXCEPTION(hResult) Graphics::GraphicsException(__LINE__, __FILE__, (hResult))
#define GRAPHICS_THROW_INFO(hResultCall) GFX_THROW_NO_INFO(hResultCall)
#define GRAPHICS_DEVICE_REMOVED_EXCEPTION(hResult) Graphics::DeviceRemovedException(__LINE__, __FILE__)
#define GRAPHICS_THROW_INFO_ONLY(call) (call)

#endif

 /*
  * Macro for importing InfoManager from Graphics into local scope
  * Also introduces hResult that is used by the above macros
  *
  * [!] this->GetInfoManager(Graphics& gfx) must exist
  */

#ifdef NDEBUG

#define INFO_MANAGER(gfx) HRESULT hResult

#else

#define INFO_MANAGER(gfx) HRESULT hResult; DebugInfoManager& infoManager = GetInfoManager((gfx))

#endif
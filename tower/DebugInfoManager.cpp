#include "DebugInfoManager.h"
#include "Window.h"
#include "Graphics.h"
#include <dxgidebug.h>
#include <memory>
#include "GraphicsMacros.h"
#include "WindowMacros.h"

#pragma comment(lib, "dxguid.lib")

DebugInfoManager::DebugInfoManager()
{
	// Define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// Load the DLL that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

	if (hModDxgiDebug == nullptr)
	{
		throw WINDOW_EXCEPTION(GetLastError());
	}

	// Get the address of DXGIGetDevugInterface in the DLL
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);

	if (DxgiGetDebugInterface == nullptr)
	{
		throw WINDOW_EXCEPTION(GetLastError());
	}

	HRESULT hResult;
	GRAPHICS_THROW_NO_INFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&debugInfoQueue)));
}

DebugInfoManager::~DebugInfoManager()
{
	if (debugInfoQueue != nullptr)
	{
		debugInfoQueue->Release();
	}
}

void DebugInfoManager::Set() noexcept
{
	nextMessageIndex = debugInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DebugInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = debugInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for (auto i = nextMessageIndex; i < end; i++)
	{
		HRESULT hResult;
		SIZE_T messageLength = 0;

		// Query the size of the current message in bytes
		GRAPHICS_THROW_NO_INFO(debugInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

		// Allocate memory for the message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		// Get the current message
		GRAPHICS_THROW_NO_INFO(debugInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}

	return messages;
}
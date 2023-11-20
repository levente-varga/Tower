#pragma once

#include "CustomWindows.h"
#include <vector>
#include <string>

class DebugInfoManager
{
public:
	DebugInfoManager();
	~DebugInfoManager();
	DebugInfoManager(const DebugInfoManager&) = delete;
	DebugInfoManager& operator=(const DebugInfoManager&) = delete;

	void Set() noexcept;
	std::vector<std::string> GetMessages() const;

private:
	unsigned long long next = 0;
	struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
};
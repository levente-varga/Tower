#pragma once

#include <memory>
#include <DirectXMath.h>
#include "Graphics.h"
#include "Bindable.h"

class Bindable;

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformMatrix() const noexcept = 0;
	void Draw(Graphics& graphics) const noexcept(!IS_DEBUG);
	virtual void Update(float dt) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> indexBuffer) noexcept;

private:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};
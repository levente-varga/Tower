#include "Drawable.h"
#include "GraphicsMacros.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics& graphics) const noexcept(!IS_DEBUG)
{
	for (auto& bindable : binds)
	{
		bindable->Bind(graphics);
	}
	graphics.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
{
	// Check if bind is an IndexBuffer, in that case, it should be added using AddIndexBuffer()
	assert("Must use AddIndexBuffer() to bind an index buffer" && typeid(*bind) != typeid(IndexBuffer));

	// std::move reallocates the resources from bind to the new list item instead of copying them 
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) noexcept
{
	// Check if an index buffer has been already added
	assert("Attempting to add an index buffer a second time" && pIndexBuffer == nullptr);

	pIndexBuffer = indexBuffer.get();
	binds.push_back(std::move(indexBuffer));
}
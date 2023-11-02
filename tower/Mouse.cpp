#include "Mouse.h"

std::pair<int, int> Mouse::GetPosition() const noexcept
{
	return { x, y };
}

int Mouse::GetPositionX() const noexcept
{
	return x;
}

int Mouse::GetPositionY() const noexcept
{
	return y;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
	return rightIsPressed;
}

bool Mouse::MiddleIsPressed() const noexcept
{
	return middleIsPressed;
}

Mouse::Event Mouse::Read() noexcept
{
	if (buffer.size() > 0)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

void Mouse::ClearQueue() noexcept
{
	buffer = std::queue<Mouse::Event>();
}

void Mouse::OnMouseMove(int newX, int newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int newX, int newY) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int newX, int newY) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnMiddlePressed(int newX, int newY) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::MPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int newX, int newY) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int newX, int newY) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnMiddleReleased(int newX, int newY) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::MRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int newX, int newY) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int newX, int newY) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferMaxSize)
	{
		buffer.pop();
	}
}

bool Mouse::QueueIsEmpty() const noexcept
{
	return buffer.empty();
}
#include "Mouse.h"
#include "CustomWindows.h"

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

bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
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

void Mouse::OnMouseLeaveWindow() noexcept
{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::LeaveWindow, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnterWindow() noexcept
{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::EnterWindow, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int newX, int newY) noexcept
{
	leftIsPressed = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int newX, int newY) noexcept
{
	rightIsPressed = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnMiddlePressed(int newX, int newY) noexcept
{
	middleIsPressed = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::MPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int newX, int newY) noexcept
{
	leftIsPressed = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int newX, int newY) noexcept
{
	rightIsPressed = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnMiddleReleased(int newX, int newY) noexcept
{
	middleIsPressed = false;
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

void Mouse::OnWheelDelta(int newX, int newY, int delta) noexcept
{
	wheelDeltaCarry += delta;
	
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
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



/*
 * Implementation of Mouse::Event
 */

Mouse::Event::Event() noexcept
	:
	type(Type::Invalid),
	leftIsPressed(false),
	rightIsPressed(false),
	middleIsPressed(false),
	x(0),
	y(0)
{}

Mouse::Event::Event(Type type, const Mouse& mouse) noexcept
	:
	type(type),
	leftIsPressed(mouse.leftIsPressed),
	rightIsPressed(mouse.rightIsPressed),
	middleIsPressed(mouse.middleIsPressed),
	x(mouse.x),
	y(mouse.y)
{}

bool Mouse::Event::IsValid() const noexcept
{
	return type != Type::Invalid;
}

Mouse::Event::Type Mouse::Event::GetType() const noexcept
{
	return type;
}

std::pair<int, int> Mouse::Event::GetPosition() const noexcept
{
	return { x, y };
}

int Mouse::Event::GetPositionX() const noexcept
{
	return x;
}

int Mouse::Event::GetPositionY() const noexcept
{
	return y;
}

bool Mouse::Event::LeftIsPressed() const noexcept
{
	return leftIsPressed;
}

bool Mouse::Event::RightIsPressed() const noexcept
{
	return rightIsPressed;
}

bool Mouse::Event::MiddleIsPressed() const noexcept
{
	return middleIsPressed;
}
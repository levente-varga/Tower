#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keyCode) const noexcept
{
	return keyStates[keyCode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keyBuffer.size() > 0)
	{
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

bool Keyboard::KeyQueueIsEmpty() const noexcept
{
	return keyBuffer.empty();
}

char Keyboard::ReadChar() noexcept
{
	if (charBuffer.size() > 0)
	{
		unsigned char charCode = charBuffer.front();
		charBuffer.pop();
		return charCode;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::CharQueueIsEmpty() const noexcept
{
	return charBuffer.empty();
}

void Keyboard::ClearKeyQueue() noexcept
{
	keyBuffer = std::queue<Event>();
}

void Keyboard::ClearCharQueue() noexcept
{
	charBuffer = std::queue<char>();
}

void Keyboard::ClearQueues() noexcept
{
	ClearKeyQueue();
	ClearCharQueue();
}

void Keyboard::EnableAutoRepeat() noexcept
{
	autoRepeatEnabled = true;
}

void Keyboard::DisableAutoRepeat() noexcept
{
	autoRepeatEnabled = false;
}

bool Keyboard::AutoRepeatIsEnabled() const noexcept
{
	return autoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keyCode) noexcept
{
	keyStates[keyCode] = true;
	keyBuffer.push(Event(Event::Type::Press, keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keyCode) noexcept
{
	keyStates[keyCode] = false;
	keyBuffer.push(Event(Event::Type::Release, keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(unsigned char character) noexcept
{
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept
{
	keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > bufferMaxSize)
	{
		buffer.pop();
	}
}



/*
 * Implementation of Keyboard::Event 
 */

Keyboard::Event::Event() noexcept
	:
	type(Type::Invalid),
	keyCode(0)
{}

Keyboard::Event::Event(Type type, unsigned char keyCode) noexcept
	:
	type(type),
	keyCode(keyCode)
{}

bool Keyboard::Event::IsPress() const noexcept
{
	return type == Type::Press;
}

bool Keyboard::Event::IsRelease() const noexcept
{
	return type == Type::Release;
}

bool Keyboard::Event::IsInvalid() const noexcept
{
	return type == Type::Invalid;
}

Keyboard::Event::Type Keyboard::Event::GetType() const noexcept
{
	return type;
}

unsigned char Keyboard::Event::GetKeyCode() const noexcept
{
	return keyCode;
}
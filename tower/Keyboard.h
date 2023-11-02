#pragma once

#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Invalid,
			Press,
			Release
		};

	private:
		Type type;
		unsigned char keyCode;

	public:
		Event() noexcept;
		Event(Type type, unsigned char keyCode) noexcept;

		bool IsPress() const noexcept;
		bool IsRelease() const noexcept;
		bool IsValid() const noexcept;
		Type GetType() const noexcept;
		unsigned char GetKeyCode() const noexcept;
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool KeyIsPressed(unsigned char keyCode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyQueueIsEmpty() const noexcept;
	void ClearKeyQueue() noexcept;

	char ReadChar() noexcept;
	bool CharQueueIsEmpty() const noexcept;
	void ClearCharQueue() noexcept;

	void ClearQueues() noexcept;

	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool AutoRepeatIsEnabled() const noexcept;

private:
	void OnKeyPressed(unsigned char keyCode) noexcept;
	void OnKeyReleased(unsigned char keyCode) noexcept;
	void OnChar(unsigned char character) noexcept;
	void ClearState() noexcept;

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

	static constexpr unsigned int nKeys = 256;
	static constexpr unsigned int bufferMaxSize = 16;
	bool autoRepeatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};
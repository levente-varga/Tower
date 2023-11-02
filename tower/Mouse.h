#pragma once

#include <queue>

class Mouse
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			Invalid,
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			WheelUp,
			WheelDown,
			Move
		};

	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		bool middleIsPressed;
		int x;
		int y;

	public:
		Event() noexcept;
		Event(Type type, const Mouse& mouse) noexcept;

		bool IsValid() const noexcept;
		Type GetType() const noexcept;
		std::pair<int, int> GetPosition() const noexcept;
		int GetPositionX() const noexcept;
		int GetPositionY() const noexcept;
		bool LeftIsPressed() const noexcept;
		bool RightIsPressed() const noexcept;
		bool MiddleIsPressed() const noexcept;
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	std::pair<int, int> GetPosition() const noexcept;
	int GetPositionX() const noexcept;
	int GetPositionY() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool MiddleIsPressed() const noexcept;
	Mouse::Event Read() noexcept;
	bool QueueIsEmpty() const noexcept;
	void ClearQueue() noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnMiddlePressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnMiddleReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;

	static constexpr unsigned int bufferMaxSize = 16;
	int x;
	int y;
	bool leftIsPressed;
	bool rightIsPressed;
	bool middleIsPressed;
	std::queue<Event> buffer;
};
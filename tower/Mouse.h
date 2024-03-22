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
			Move,
			EnterWindow,
			LeaveWindow
		};

	private:
		Type type;
		bool isLeftPressed;
		bool isRightPressed;
		bool isMiddlePressed;
		int positionX;
		int positionY;

	public:
		Event() noexcept;
		Event(Type type, const Mouse& mouse) noexcept;

		bool IsValid() const noexcept;
		Type GetType() const noexcept;
		std::pair<int, int> GetPosition() const noexcept;
		int GetPositionX() const noexcept;
		int GetPositionY() const noexcept;
		bool IsLeftPressed() const noexcept;
		bool IsRightPressed() const noexcept;
		bool IsMiddlePressed() const noexcept;
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	std::pair<int, int> GetPosition() const noexcept;
	int GetPositionX() const noexcept;
	int GetPositionY() const noexcept;
	bool IsInWindow() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	bool IsMiddlePressed() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsQueueEmpty() const noexcept;
	void ClearQueue() noexcept;
	int wheelDeltaCarry = 0;
private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseLeaveWindow() noexcept;
	void OnMouseEnterWindow() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnMiddlePressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnMiddleReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;

	static constexpr unsigned int bufferMaxSize = 16;
	int positionX = -1;
	int positionY = -1;
	bool isInWindow;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool middleIsPressed = false;

	std::queue<Event> buffer;
};
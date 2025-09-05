// Platform/Win32/Window.h

#pragma once
#include <Windows.h>
#include <string>

/**
 * @class Window
 * @brief Abstraction for a Win32 Window.
 *
 * This class encapsulates the creation and basic management of a Win32 window,
 * including message polling and window display. It provides a simple interface
 * to interact with the native HWND handle.
 */
class Window
{
public:
	/**
	 * @brief Constructs an uninitialized Window object.
	 */
	Window();

	/**
	 * @brief Destructor. Destroys the window if it was created.
	 */
	~Window();

	/**
	 * @brief Creates a Win32 window.
	 *
	 * This function registers a window class (if necessary) and creates a native
	 * window with the specified title and dimensions.
	 *
	 * @param title The title of the window (shown in the title bar).
	 * @param width The width of the window in pixels.
	 * @param height The height of the window in pixels.
	 * @return true if the window was successfully created, false otherwise.
	 */
	bool Create(const char* title, int width, int height);

	/**
	 * @brief Displays the window on the screen.
	 */
	void Show();

	/**
	 * @brief Polls and processes pending window messages (e.g., input, close, resize).
	 *
	 * This should be called regularly (e.g., every frame in a game loop) to ensure
	 * the window responds to user input and system events.
	 */
	void PollEvents();

	/**
	 * @brief Retrieves the native Win32 window handle.
	 *
	 * @return The HWND associated with this window.
	 */
	HWND GetHandle() const { return hwnd_; }

private:
	HWND hwnd_; ///< Native Win32 window handle.

	/**
	 * @brief The static window procedure (WndProc) used by Win32 to dispatch messages.
	 *
	 * This function is called by the OS whenever the window receives a message (e.g., WM_CLOSE).
	 * It must be static or a free function to match the Win32 API signature.
	 *
	 * @param hwnd Handle to the window receiving the message.
	 * @param uMsg The message identifier.
	 * @param wParam Additional message information (depends on message).
	 * @param lParam Additional message information (depends on message).
	 * @return The result of message processing (typically via DefWindowProc).
	 */
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
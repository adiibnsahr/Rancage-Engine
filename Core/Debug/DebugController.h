// Core/Debug/DebugController.h
#pragma once

/**
 * @file DebugController.h
 * @brief Declares the DebugController class for managing runtime debug toggle input.
 */

 /**
  * @class DebugController
  * @brief Handles user input or system triggers to enable or disable debug mode/features.
  *        Useful for runtime toggling of debugging tools or visualizations.
  */
class DebugController {
public:
    /**
     * @brief Constructs the DebugController with debug mode disabled by default.
     */
    DebugController() : debugEnabled(false) {}

    /**
     * @brief Updates internal state based on input or system flags.
     *        Should be called once per frame to check for toggle requests.
     */
    void Update();

    /**
     * @brief Checks if debug mode is currently enabled.
     * @return True if debug features should be active, false otherwise.
     */
    bool IsDebugEnabled() const { return debugEnabled; }

private:
    /**
     * @brief Indicates whether debug mode is currently enabled.
     */
    bool debugEnabled;
};

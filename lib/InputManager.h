#pragma once

#include "Common.h"

/**
 * @brief The InputManager class handles input events such as keyboard and mouse interactions.
 * same as EventManager!!!
 */
class InputManager{
	public:
		/**
		 * @brief A map that stores the state of each key.
		 */
		static inline std::map<int, bool> keysState;

		/**
		 * @brief A struct that represents the state of the mouse.
		 */
		typedef struct {
			double posX, posY;
			std::map<int, bool> buttons;
		} mouseState_t;

		/**
		 * @brief The current state of the mouse.
		 */
		static inline mouseState_t mouseState = { 0, 0 };

		/**
		 * @brief Handles mouse button events.
		 * 
		 * @param window The GLFW window.
		 * @param button The mouse button.
		 * @param action The action (e.g., press, release).
		 * @param mods The modifier keys.
		 */
		static void mouseButtonManager(GLFWwindow* window, int button, int action, int mods);

		/**
		 * @brief Handles mouse position events.
		 * 
		 * @param window The GLFW window.
		 * @param x The x-coordinate of the mouse position.
		 * @param y The y-coordinate of the mouse position.
		 */
		static void mousePosManager(GLFWwindow* window, double x, double y);

		/**
		 * @brief Handles keyboard events.
		 * 
		 * @param window The GLFW window.
		 * @param key The keyboard key.
		 * @param scancode The scancode.
		 * @param action The action (e.g., press, release).
		 * @param mods The modifier keys.
		 */
		static void keyboardManager(GLFWwindow* window, int key, int scancode, int action, int mods);

		/**
		 * @brief Initializes the InputManager.
		 * 
		 * @param window The GLFW window.
		 */
		static void initInputManager(GLFWwindow* window);
};


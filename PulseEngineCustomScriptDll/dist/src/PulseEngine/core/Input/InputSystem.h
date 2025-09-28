#include <unordered_map>
#include <string>
#include <iostream>

// A simple input system supporting keyboard and mouse mapping
// - No backend dependencies, you feed events manually
// - Supports action mapping (string name -> key/button)
// - Per-frame pressed/released/down queries

#define LEFT_MOUSE_BUTTON 0
#define RIGHT_MOUSE_BUTTON 1

class InputSystem {
public:
    enum class KeyState {
        Up,
        Down,
        Pressed,
        Released
    };

    // Call at the beginning of each frame to reset transient states
    void newFrame() {
        for (auto &kv : keyStates) {
            if (kv.second == KeyState::Pressed) kv.second = KeyState::Down;
            else if (kv.second == KeyState::Released) kv.second = KeyState::Up;
        }
        for (auto &kv : mouseStates) {
            if (kv.second == KeyState::Pressed) kv.second = KeyState::Down;
            else if (kv.second == KeyState::Released) kv.second = KeyState::Up;
        }
    }

    // Inject raw events ---------------------------------------------------
    void onKeyEvent(int key, bool down) {
        KeyState &st = keyStates[key];
        if (down) {
            if (st == KeyState::Up || st == KeyState::Released) st = KeyState::Pressed;
        } else {
            if (st == KeyState::Down || st == KeyState::Pressed) st = KeyState::Released;
        }
    }

    void onMouseButtonEvent(int button, bool down) {
        KeyState &st = mouseStates[button];
        if (down) {
            if (st == KeyState::Up || st == KeyState::Released) st = KeyState::Pressed;
        } else {
            if (st == KeyState::Down || st == KeyState::Pressed) st = KeyState::Released;
        }
    }

    void onMouseMove(double x, double y) {
        mouseX = x;
        mouseY = y;
    }

    // Mapping API ----------------------------------------------------------
    void bindAction(const std::string &name, int key) {
        keyBindings[name] = key;
    }

    void bindMouseAction(const std::string &name, int button) {
        mouseBindings[name] = button;
    }

    // Query API ------------------------------------------------------------
    bool isActionDown(const std::string &name) const {
        return check(name, KeyState::Down) || check(name, KeyState::Pressed);
    }

    bool wasActionPressed(const std::string &name) const {
        return check(name, KeyState::Pressed);
    }

    bool wasActionReleased(const std::string &name) const {
        return check(name, KeyState::Released);
    }

    double getMouseX() const { return mouseX; }
    double getMouseY() const { return mouseY; }

private:
    std::unordered_map<int, KeyState> keyStates;
    std::unordered_map<int, KeyState> mouseStates;
    std::unordered_map<std::string, int> keyBindings;
    std::unordered_map<std::string, int> mouseBindings;

    double mouseX = 0.0, mouseY = 0.0;

    bool check(const std::string &name, KeyState st) const {
        auto it = keyBindings.find(name);
        if (it != keyBindings.end()) {
            auto ks = keyStates.find(it->second);
            if (ks != keyStates.end() && ks->second == st) return true;
        }
        auto im = mouseBindings.find(name);
        if (im != mouseBindings.end()) {
            auto ms = mouseStates.find(im->second);
            if (ms != mouseStates.end() && ms->second == st) return true;
        }
        return false;
    }
};

// Example usage:
// InputSystem input;
// input.bindAction("jump", ' '); // space key
// input.bindMouseAction("shoot", 0); // left mouse button
//
// In your loop:
//   input.newFrame();
//   input.onKeyEvent(' ', true);
//   if (input.wasActionPressed("jump")) { ... }

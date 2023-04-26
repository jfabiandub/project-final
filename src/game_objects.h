#include <iostream>
#include <OpenGL/gl.h>

class GameOverWindow : public Window {
public:
    GameOverWindow(
        const std::string& message,
        const std::function<void()>& onStartCallback,
        float x = 0.0f,
        float y = 0.0f,
        float width = 400.0f,
        float height = 300.0f
    ) : m_message(message), m_onStartCallback(onStartCallback) {
        setWindowPos(x, y);
        setWindowSize(width, height);
    }

    void setup() override {
        setClearColor(vec3(0.9f, 0.9f, 0.9f));
    }

    void draw() override {
        clearScreen();
        drawText(m_message, 0.0f, 0.1f, 1.0f);
        bool start_button_pressed = button("Start", 0.0f, -0.1f, 0.5f);
        if (start_button_pressed) {
            m_onStartCallback();
            close();
        }
    }

    void onMouseButton(int button, int action, int mods) override {
        Window::onMouseButton(button, action, mods);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double x, y;
            getMousePos(x, y);
            if (x >= -0.25f && x <= 0.25f && y >= -0.15f && y <= -0.05f) {
                m_onStartCallback();
                close();
            }
        }
    }

private:
    std::string m_message;
    std::function<void()> m_onStartCallback;
};
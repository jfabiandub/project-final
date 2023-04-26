#pragma once

#include <agl/window.h>
#include <functional>
#include <string>

class GameOverWindow : public agl::Window {
public:
  GameOverWindow(const std::string& message,
                 const std::function<void()>& onStartCallback);

  void setup();

  void draw();

  void onMouseButton(int button, int action, int mods);

private:
  std::string m_message;
  std::function<void()> m_onStartCallback;
  bool m_isStartButtonClicked = false;
};

GameOverWindow::GameOverWindow(const std::string& message,
                               const std::function<void()>& onStartCallback)
  : m_message(message),
    m_onStartCallback(onStartCallback) {
}

void GameOverWindow::setup() {
  setWindowSize(300, 200);
}

void GameOverWindow::draw() {
  clearScreen();

  drawText(m_message, -0.8f, 0.2f, 1.0f);

  bool start_button_pressed = button("Play Again", -0.2f, -0.2f, 0.5f);

  if (start_button_pressed && !m_isStartButtonClicked) {
    m_isStartButtonClicked = true;
    m_onStartCallback();
  }
}

void GameOverWindow::onMouseButton(int button, int action, int mods) {
  m_isStartButtonClicked = false;
}
#ifndef GAMEELEMENTS_H
#define GAMEELEMENTS_H

#include <glm/glm.hpp>

class Ball {
public:
  float radius;
  glm::vec2 position;
  glm::vec2 velocity;
  glm::vec4 color;

  Ball(float r, glm::vec2 pos, glm::vec2 vel, glm::vec4 col)
    : radius(r), position(pos), velocity(vel), color(col) {}
};

class Paddle {
public:
  glm::vec2 position;
  glm::vec2 size;
  glm::vec4 color;

  Paddle(glm::vec2 pos, glm::vec2 s, glm::vec4 col)
    : position(pos), size(s), color(col) {}
};

#endif
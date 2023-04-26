#pragma once
#include <glm/glm.hpp>
#include "game_objects.h"

class Ball {
public:
  Ball(float radius, glm::vec2 position, glm::vec2 velocity, glm::vec4 color);
  ~Ball();

  void move(float delta_time);
  void draw(){
  renderer.push();
  renderer.translate(vec3(position, 0));
  renderer.scale(vec3(radius));
  renderer.color(color);
  renderer.circle();
  renderer.pop();
  }

  glm::vec2 position;
  glm::vec2 velocity;
  float radius;
  glm::vec4 color;
};

class Paddle {
public:
  Paddle(glm::vec2 position, glm::vec2 size, glm::vec4 color);
  ~Paddle();

  void move_left(float delta_time);
  void move_right(float delta_time);
  void draw(){
  renderer.push();
  renderer.translate(vec3(position, 0));
  renderer.scale(vec3(size.x, size.y, 1.0f));
  renderer.color(color);
  renderer.rect();
  renderer.pop();
  }

  glm::vec2 position;
  glm::vec2 size;
  glm::vec4 color;
};
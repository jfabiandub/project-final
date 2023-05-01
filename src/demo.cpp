// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"
#include <functional>
#include <string>
#include <random>



using namespace std;
using namespace glm;
using namespace agl;


class Ball {
public:
  vec2 position;
  vec2 velocity;
  float radius;

};

class Paddle {
public:
  vec2 position;
  vec2 size;
  float speed;
  vec4 color;
};

class Brick {
public:
  vec2 position;
  vec2 size;
  bool is_alive;
};


class Viewer : public Window {
public:
  Ball ball; 

  Paddle paddle;

  std::vector<Brick> bricks;

  bool game_over = false;
  bool ball_lost = false;
  bool restart_requested = false;

  bool last_brick_hit = false;

  int texture_index = 0;

  int level = 1;

   int num_ball_lost = 0;


Viewer() : Window() {
}

void setup() {
  setWindowSize(1000, 1000);

renderer.loadShader("simple-T", "../shaders/simple-T.vs", "../shaders/simple-T.fs");
renderer.loadTexture("title3", "../textures/title3.png", 0);
renderer.loadTexture("white", "../textures/white.png", 0);
renderer.loadTexture("blue", "../textures/blue.png", 0);
renderer.loadTexture("marron", "../textures/marron.png", 0);
renderer.loadTexture("yellow", "../textures/yellow.png", 0);
renderer.loadTexture("green", "../textures/green.png", 0);
renderer.loadTexture("blue2", "../textures/blue2.png", 0);
renderer.loadTexture("gray", "../textures/gray.png", 0);
renderer.loadTexture("turquise", "../textures/turquise.png", 0);




ball.position = vec2(0, -0.8);
ball.velocity = normalize(vec2(0.5, 1.0)) * 0.7f;
ball.radius = 0.05f;
    
paddle.position = vec2(0, -0.9);
paddle.size = vec2(0.3, 0.05);
paddle.speed = 0.5f;
paddle.color = vec4(0.0f, 1.0f, 1.0f, 1.0f);

createBricks();

}


void createBricks() {
  bricks.clear();

  int num_bricks = level == 1 ? 10 : level == 2 ? 20 : 30; // set the number of bricks based on the level
  float brick_width = 0.1f;
  float brick_height = 0.05f;
  float brick_gap = 0.01f;
  float bricks_start_x = -0.45f;
  float bricks_start_y = level == 1 ? 0.5f : level == 2 ? 0.8f : 0.9f; // set the y-position of the bricks based on the level

  if (level == 2 || level == 3) {
    ball.velocity *= 1.5f; // increase the ball speed for levels 2 and 3
    paddle.size /= 2.0f; // decrease the paddle size for level 2 and 3
  }
  if (level == 3) {
    ball.velocity *= 1.5f; // increase the ball speed for level 3
    paddle.size *= 2.0f; // increase the paddle size for level 3
  }

  for (int i = 0; i < num_bricks; i++) {
    bricks.push_back({
      vec2(
        bricks_start_x + (brick_width + brick_gap) * (i % 10),
        bricks_start_y - (brick_height + brick_gap) * (i / 10)
      ),
      vec2(brick_width, brick_height),
      true
    });
  }
}
 
void update(float dt) {
      // check for collisions with the paddle and bricks
  bool hit_paddle = checkBallPaddleCollision();
  bool hit_brick = checkBallBrickCollision();
  
  if (hit_paddle || hit_brick) {
    // update the ball's position
    updateBallPosition(dt);

    if (bricks.back().is_alive == false && level == 1) {
      last_brick_hit = true;
    }
  }

  // check if the ball went beyond the paddle
 if (ball.position.y - ball.radius < paddle.position.y - paddle.size.y) {
        ball_lost = true;
        num_ball_lost++; // increment ball lost counter
        if (num_ball_lost >= 3) { // check if game should restart from level 1
            restart_requested = true;
            num_ball_lost = 0;
        } else {
            restartGame(); // restart current level
        }
    }
    // check if the game is over
    if (bricks.size() == 0) {
        game_over = true;
        level++; // increase the level
        restartGame(); // restart the game with new level
    }

}


 void restartGame() {
   ball.position = vec2(0, -0.8);
  ball.velocity = normalize(vec2(0.5, 1.0)) * 0.7f;
  ball_lost = false;
  game_over = false;
  bricks.clear();
  if (restart_requested) { // if restarting from level 1
    level = 1;
    num_ball_lost = 0;
    restart_requested = false;
  }
  createBricks();
  }

bool checkBallPaddleCollision() {
  if (ball.position.y - ball.radius < paddle.position.y + paddle.size.y &&
    ball.position.x > paddle.position.x - paddle.size.x / 2.0f &&
    ball.position.x < paddle.position.x + paddle.size.x / 2.0f &&
    ball.velocity.y < 0) {  // ball is moving downwards
    ball.velocity.y = -ball.velocity.y;  // reverse y-velocity

    index++;
    index = index % 6;
    return true;
    }
  return false;
}
  

bool checkBallBrickCollision() {
  for (int i = 0; i < bricks.size(); i++) {
    if (bricks[i].is_alive &&
      ball.position.y + ball.radius > bricks[i].position.y - bricks[i].size.y / 2 &&
          ball.position.y - ball.radius < bricks[i].position.y + bricks[i].size.y / 2 &&
          ball.position.x > bricks[i].position.x - bricks[i].size.x / 2 &&
          ball.position.x < bricks[i].position.x + bricks[i].size.x / 2) {
        bricks[i].is_alive = false;
        
         if (i == bricks.size() - 1) {
          last_brick_hit = true;
        }

        vec2 v_norm = normalize(ball.velocity);
        vec2 h = normalize(vec2(bricks[i].position.x - ball.position.x, 0.0f));
        vec2 v1 = 2.0f * dot(h, v_norm) * h - v_norm;
        ball.velocity = v1 * length(ball.velocity);
        return true;
      }
    }
    return false;
  }

  void updateBallPosition(float dt) {
   ball.position += ball.velocity * dt;

  // Check if ball is going to hit the right wall
  float right_wall_pos = 0.55f; // Set the position of the right wall
  float left_wall_pos = -0.55f; // Set the position of the right wall
  
  if (ball.position.x + ball.radius > right_wall_pos) {
    // Reflect the ball's velocity and set its position to the wall minus the radius
    ball.velocity.x = -ball.velocity.x;
    ball.position.x = right_wall_pos - ball.radius;
    
  }
  // Check if ball is going to hit the left wall
  if (ball.position.x - ball.radius < left_wall_pos) {
    // Reflect the ball's velocity and set its position to the wall plus the radius
    ball.velocity.x = -ball.velocity.x;
    ball.position.x = left_wall_pos + ball.radius;
  }

  // Check if ball is going to hit the top wall
  float top_wall_pos = 0.9f; // Set the position of the top wall
  if (ball.position.y + ball.radius > top_wall_pos) {
    // Reflect the ball's velocity and set its position to the wall minus the radius
    ball.velocity.y = -ball.velocity.y;
    ball.position.y = top_wall_pos - ball.radius;
  }

  // Check if ball is going to hit the paddle
  if (checkBallPaddleCollision()) {
    ball.position.y = paddle.position.y + paddle.size.y / 2 + ball.radius;
  }

  // Check if ball is going to hit a brick
  if (checkBallBrickCollision()) {
    // Do nothing
  }

  // Check if ball has gone beyond the paddle
  float bottom_wall_pos = -1.0f; // Set the position of the bottom wall
  if (ball.position.y - ball.radius < bottom_wall_pos) {
    game_over = true;
  }
}

  void keyEvent(int key, double dt) {
  if (key == GLFW_KEY_LEFT) {
    paddle.position.x -= paddle.speed * dt;
  } else if (key == GLFW_KEY_RIGHT) {
    paddle.position.x += paddle.speed * dt;
  }
}

void updatePaddlePosition(float dt) {
  float newXPos = paddle.position.x;
  float leftBound = -1.0f + paddle.size.x / 2;
  float rightBound = 1.0f - paddle.size.x / 2;
  if (glfwGetKey(window(), GLFW_KEY_LEFT) == GLFW_PRESS) {
    newXPos = paddle.position.x - paddle.speed * dt;
  } else if (glfwGetKey(window(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
    newXPos = paddle.position.x + paddle.speed * dt;
  }
  // Check if paddle would go out of bounds, if not, move paddle
  if (newXPos >= leftBound && newXPos <= rightBound) {
    paddle.position.x = newXPos;
  }
}





  void draw() {
 
   //draw border
  renderer.push();
  renderer.translate(vec3(0.0f, 0.0f, -0.1f));  // Move the border slightly behind other objects
  renderer.scale(vec3(1.5f, 2.0f, 0.1f));  // Scale the border to cover the entire screen
  //renderer.color(0.0f, 0.0f, 0.0f, 1.0f);  // Set the border color (black)
  renderer.beginShader("simple-T");
  renderer.texture("image", "turquise");
  renderer.cube();
  renderer.pop();
  renderer.endShader();
  

  updateBallPosition(dt());


    //draws the ball
  renderer.push();
  renderer.translate(vec3(ball.position, 0));
  renderer.scale(vec3(ball.radius));
  renderer.beginShader("simple-T");
  renderer.texture("image", "gray");
  renderer.sphere();
  renderer.pop();
    

  updatePaddlePosition(dt());
    // draw paddle 

  renderer.push();
  renderer.translate(vec3(paddle.position, 0));
  renderer.scale(vec3(paddle.size.x, paddle.size.y, 0.1f));
 // renderer.color(0.5f, 0.5f, 0.5f); //set color to gray
  renderer.beginShader("simple-T");
  renderer.texture("image", shaders[index]);


  renderer.cube();
  renderer.pop();
  renderer.endShader();
  
  
    //draw bricks 
  for (int i = 0; i < bricks.size(); i++) {
    if (bricks[i].is_alive) {
      renderer.push();
      renderer.translate(vec3(bricks[i].position, 0));
      renderer.scale(vec3(bricks[i].size, 0));
      renderer.cube();
      renderer.pop();

    }
  }
  

  checkBallBrickCollision();
  checkBallPaddleCollision();
  update(dt());
}


protected:
  bool allDead = false;
  bool animation_started = false;


vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);
  int frames = 0;


  int index = 0;
  std::vector<string> shaders = {"white", "blue", "marron", "yellow", "green", "blue2"};
  

};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
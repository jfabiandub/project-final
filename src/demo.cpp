// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"
//#include "game_over_window.h"
#include <functional>
#include <string>




using namespace std;
using namespace glm;
using namespace agl;



class Ball {
public:
  vec2 position;
  vec2 velocity;
  float radius;
  ///vec4 color;
};

class Paddle {
public:
  vec2 position;
  vec2 size;
  float speed;
};

class Brick {
public:
  vec2 position;
  vec2 size;
  bool is_alive;
};



class Viewer : public Window {
public:

enum class GameState {
  PLAYING,
  GAME_OVER,
};

  GameState state = GameState::PLAYING;
  Ball ball; 

  Paddle paddle;

  std::vector<Brick> bricks;

  bool game_over = false;
  bool ball_lost = false;
  bool restart_requested = false;


  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);

    ball.position = vec2(0, -0.8);
    ball.velocity = normalize(vec2(0.5, 1.0)) * 0.7f;
    ball.radius = 0.05f;
    //ball.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    
    paddle.position = vec2(0, -0.9);
    paddle.size = vec2(0.3, 0.05);
    paddle.speed = 0.5f;
    //paddle.color = vec4(0.0f, 0.0f, 1.0f, 1.0f);

    const int num_bricks = 10;
    const float brick_width = 0.1f;
    const float brick_height = 0.05f;
    const float brick_gap = 0.01f;
    const float bricks_start_x = -0.45f;
    const float bricks_start_y = 0.5f;
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
    switch (state) {
      case GameState::PLAYING: {
        // check for collisions with the paddle and bricks
        bool hit_paddle = checkBallPaddleCollision();
        bool hit_brick = checkBallBrickCollision();

        // update the ball's position
        updateBallPosition(dt);

        // check if the ball went beyond the paddle
        if (ball.position.y - ball.radius < paddle.position.y - paddle.size.y) {
          state = GameState::GAME_OVER;
          ball_lost = true;
        }

        // check if all bricks are destroyed
        bool all_bricks_destroyed = true;
        for (int i = 0; i < bricks.size(); i++) {
          if (bricks[i].is_alive) {
            all_bricks_destroyed = false;
            break;
          }
        }
        if (all_bricks_destroyed) {
          state = GameState::GAME_OVER;
        }
        if (state == GameState::GAME_OVER) {
          //showGameOverWindow();
        }
        break;
      }
      case GameState::GAME_OVER: {
        break;
      }
    }
  }

/*
  void showGameOverWindow() {
  clearScreen();
  drawText("Game Over", 0.0f, 0.0f, 1.0f);

  GameOverWindow gameOverWindow("Play again?", [&]() {
    restart_requested = true;
    game_over = false;
  });
  gameOverWindow.setup();
  gameOverWindow.draw();
}
*/
  void restartGame() {
  ball.position = vec2(0, -0.8);
  ball.velocity = normalize(vec2(0.5, 1.0)) * 0.7f;
  ball_lost = false;
  game_over = false;
  state = GameState::PLAYING;
  bricks.clear();
  const int num_bricks = 10;
  const float brick_width = 0.1f;
  const float brick_height = 0.05f;
  const float brick_gap = 0.01f;
  const float bricks_start_x = -0.45f;
  const float bricks_start_y = 0.5f;
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

bool checkBallPaddleCollision() {
  if (ball.position.y - ball.radius < paddle.position.y + paddle.size.y &&
    ball.position.x > paddle.position.x - paddle.size.x / 2.0f &&
    ball.position.x < paddle.position.x + paddle.size.x / 2.0f &&
    ball.velocity.y < 0) {  // ball is moving downwards
    ball.velocity.y = -ball.velocity.y;  // reverse y-velocity
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



  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
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


    //clearScreen();

  if (game_over) {
    // Draw "Game Over" message and "Start" button
    //drawText("Game Over", -0.1f, 0.0f, 0.1f);
    //if (button("Start", 0.0f, -0.2f, 0.1f)) {
      // Restart the game
      game_over = false;
      ball.position = vec2(0, -0.8);
      ball.velocity = normalize(vec2(0.5, 1.0)) * 0.7f;
      paddle.position = vec2(0, -0.9);
      for (int i = 0; i < bricks.size(); i++) {
        bricks[i].is_alive = true;
      }
    //}
  }
  float aspect = ((float)width()) / height();
  renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
  renderer.lookAt(vec3(0,0,2), vec3(0), vec3(0,1,0));

  updateBallPosition(dt());
    //draws the ball
  renderer.push();
  renderer.translate(vec3(ball.position, 0));
  renderer.scale(vec3(ball.radius));
  renderer.sphere();
  renderer.pop();
    

  updatePaddlePosition(dt());
    // draw paddle 
  renderer.push();
  renderer.translate(vec3(paddle.position, 0));
  renderer.scale(vec3(paddle.size.x, paddle.size.y, 0.1f));
 // renderer.color(0.5f, 0.5f, 0.5f); //set color to gray
  renderer.cube();
  renderer.pop();
  
  
    //draw bricks 
  for (int i = 0; i < bricks.size(); i++) {
    if (bricks[i].is_alive) {
      renderer.push();
      renderer.translate(vec3(bricks[i].position, 0));
      renderer.scale(vec3(bricks[i].size, 0));
      //renderer.color(1.0f, 0.0f, 0.0f); //set color to red
      renderer.cube();
      renderer.pop();



    }
  }

  checkBallBrickCollision();
  checkBallPaddleCollision();
}

protected:
  vec3 position = vec3(1, 0, 0);


};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
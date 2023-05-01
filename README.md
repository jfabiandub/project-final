# Atari Breakout Game

Atari Breakout is a classic game where the player must use a paddle to bounce a ball against a wall of bricks. The goal is to break all the bricks without letting the ball fall off the bottom of the screen. This project is an implementation of this game using the AGL library.

<img width="634" alt="Screenshot 2023-05-01 at 2 55 04 PM" src="https://user-images.githubusercontent.com/102636198/235510840-479e13c6-5329-49ed-8242-22d983c639c0.png">

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
project-template $ mkdir build
project-template $ cd build
project-template/build $ cmake ..
project-template/build $ start project-final.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
project-template/build $ ../bin/Debug/demo.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
project-template $ mkdir build
project-template $ cd build
project-template/build $ cmake ..
project-template/build $ make
```

To run each program from build, you would type

```
project-template/build $ ../bin/demo
```

TODO: Document the algorithims and Data Structures 
## Agorithims and Data structures 

Collision detection algorithm: Used to detect collisions between the ball and other game objects such as the paddle and bricks.
Game loop algorithm: Used to continuously update the game state and render the game on the screen.
Vector math: Used extensively to calculate positions, velocities, and other properties of game objects.
Data structures: Used to store information about game objects, such as their position, size, and state. The following data structures are used in the game:
Ball: Stores information about the ball, such as its position, velocity, and radius.
Paddle: Stores information about the paddle, such as its position, size, and speed.
Brick: Stores information about a brick, such as its position, size, and whether it is still alive or not.

## Demo of basic features

The game consists of a ball, a paddle, and multiple bricks. The player controls the paddle using the left and right arrow keys. The objective of the game is to bounce the ball off the paddle and destroy all the bricks on the screen.

## Unique features 

TODO: Show the unique features you made


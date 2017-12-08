// name: Luke Heary
// file: main.h
// date: 11/14/17

#include <stdbool.h>

typedef struct {

  int robotNumber;
  int num;
  int initialR;
  int initialC;
  int currentR;
  int currentC; 
  int numberOfMoves;

  int rows;
  int columns;
  int *cellsAround;
  int **grid;

  int targetR;
  int targetC;

  bool atTarget;

  //bool active;
  //bool passive;
  bool touchingRobot;

} robot;



int **createGrid(int rows, int columns, int targetR, int targetC);
void printGrid(int length, int width, int **grid);

robot * createRobots(int numberOfRobots, int length, int width, int **grid, pthread_t *robots);
void moveRobot(robot *r, int numberOfRobots);
bool robotSearchAround(robot *r);
void alert(robot *robots, int numberOfRobots, int targetR, int targetC); 
void moveRobotsToTarget(robot *r,int numberOfRobots); 

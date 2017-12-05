// name: Luke Heary
// file: main.h
// date: 11/14/17

#include <stdbool.h>

typedef struct {

  int robotNumber;
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

  bool btml;

} robot;

int **createGrid(int rows, int columns, int targetR, int targetC);
void printGrid(int length, int width, int **grid);

robot * createRobots(int numberOfRobots, int length, int width, int **grid);
bool moveRobot(robot *r, robot *robots, int numberOfRobots);
void robotSearchAround(robot *r);
void alert(robot *robots, int numberOfRobots, int targetR, int targetC); 

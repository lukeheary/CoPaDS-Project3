// name: Luke Heary
// file: main.h
// date: 11/14/17

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

} robot;

int **createGrid(int length, int width);
void printGrid(int length, int width, int **grid);

robot * createRobots(int numberOfRobots, int length, int width, int **grid);
int **moveRobot(robot *r, robot *robots, int numberOfRobots);
void robotSearchAround(robot *r);

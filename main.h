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

} robot;

int **createGrid(int length, int width);
void printGrid(int length, int width, int **grid);

pthread_t * createRobots(int numberOfRobots, int length, int width, int **grid);
void moveRobot(robot *r);

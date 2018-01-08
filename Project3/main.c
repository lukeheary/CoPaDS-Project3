// name: Luke Heary
// email: ldh4586@rit.edu
// file: main.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "main.h"
#include <unistd.h>

// creates the grid and places the target at a random point
//
// @param rows			the number of rows inputted by user
// @param columns		the number of columns inputted
// @param targetR		the row number for the target
// @param targetC		the column number for the target 
// @return grid			returns the finished grid
int **createGrid(int rows, int columns, int targetR, int targetC) {
  
  int **grid = (int **)calloc(rows, rows * sizeof(int));
  for(int i = 0; i < rows; i++) {
    grid[i] = (int *)calloc(columns, columns * sizeof(int));
  }

  printf("\nTarget Coordinates: %d, %d \n\n", targetR, targetC);
  grid[targetR][targetC] = 1;

  return grid;
}

// prints the grid
//
// @param rows			number of rows in the grid
// @param columns		number of columns in the grid
// @param **grid		the pointer to the grid
void printGrid(int rows, int columns, int **grid) {

  printf("\n   ");
  for(int i = 0; i < columns; i++) {
    //printf("%d  ", i);
  }
  printf("\n");

  for(int x = 0; x < rows; x++) {
    //printf("%d  ", x);
    for(int y = 0; y < columns; y++) {
      if(grid[x][y] == 1) {
        printf("T  ");
      } else if(grid[x][y] == 2) {
        printf("R  ");
      } else if(grid[x][y] == 3) {
        printf("*  ");
      } else {
        printf(".  ");
      }
    }

    printf("\n");
  }
  printf("\n");
}

// checks to see if all the robots are at the target
//
// @param *robots		the list of robots
// @param numberOfRobots	the number of robots
bool checkRobots(robot *robots, int numberOfRobots) {

  for(int i = 0; i < numberOfRobots; i++) {
    if(robots[i].atTarget == false) {
      return false;
    }
  }
  return true;
}

// gets the user input and runs the program
int main(void) {

  int rows, columns, numberOfRobots;

  printf("Enter the number of rows: ");
  scanf("%i", &rows);
  printf("Enter the number of columns: ");
  scanf("%i", &columns);
  printf("Enter the number of robots: ");
  scanf("%i", &numberOfRobots);

  clock_t begin = clock();
 
  srand(time(NULL));
  int targetR = rand() % rows;
  int targetC = rand() % columns;

  pthread_t *robotThreads;
  robotThreads = (pthread_t *)malloc(numberOfRobots * sizeof(pthread_t));

  int **grid = createGrid(rows, columns, targetR, targetC);
  robot *robots = createRobots(numberOfRobots, rows, columns, grid, robotThreads);

  //initial print without any searchs
  printGrid(rows, columns, grid);
  usleep(400000);

  bool targetFound = false;

  //initial look around
  for(int robotNum = 0; robotNum < numberOfRobots; robotNum++) {
    targetFound = robotSearchAround(&robots[robotNum]);
    if(targetFound == true) { break; };
  }

  printGrid(rows, columns, grid);
  sleep(0);
  printf("\e[1;1H\e[2J"); 

  //move robots and look around
  while(targetFound == false) {
    //for loop for individual robot move
    for(int robotNum = 0; robotNum < numberOfRobots; robotNum++) {    
      if(targetFound == false) {  
        moveRobot(&robots[robotNum], numberOfRobots);
        targetFound = robotSearchAround(&robots[robotNum]); 
      }
    }
    printGrid(rows, columns, grid);
    usleep(400000);
    printf("\e[1;1H\e[2J");
  }

  //TARGET FOUND
  alert(robots, numberOfRobots, targetR, targetC);
  sleep(1);

  bool allRobotsAtTarget = checkRobots(robots, numberOfRobots);

  while(allRobotsAtTarget == false) {
    for(int robotNum = 0; robotNum < numberOfRobots; robotNum++) {
      moveRobotsToTarget(&robots[robotNum], numberOfRobots);
    }
    printGrid(rows, columns, grid);
    usleep(400000);
    printf("\e[1;1H\e[2J");
    allRobotsAtTarget = checkRobots(robots, numberOfRobots);
  }


  //print out statistics
  printf("\n");

  clock_t end = clock();
  double time = (double)(end-begin) / CLOCKS_PER_SEC;

  printGrid(rows, columns, grid);

  printf("Target surrounded in %f seconds. Number of moves:\n", time);
  for(int i = 0; i < numberOfRobots; i++) {

    printf("Robot Number %d: %d moves\n", robots[i].num, robots[i].numberOfMoves);    

  }

  for(int i = 0; i < numberOfRobots; i++) {
    pthread_join(robotThreads[i], NULL);
  }

}

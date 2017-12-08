// name: Luke Heary
// email: ldh4586@rit.edu
// file: main.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "main.h"


int **createGrid(int rows, int columns, int targetR, int targetC) {
  
  int **grid = (int **)calloc(rows, rows * sizeof(int));
  for(int i = 0; i < rows; i++) {
    grid[i] = (int *)calloc(columns, columns * sizeof(int));
  }

  printf("\nTarget Coordinates: %d, %d \n\n", targetR, targetC);
  grid[targetR][targetC] = 1;

  return grid;
}


void printGrid(int rows, int columns, int **grid) {

  printf("\n   ");
  for(int i = 0; i < columns; i++) {
    printf("%d  ", i);
  }
  printf("\n");

  for(int x = 0; x < rows; x++) {
    printf("%d  ", x);
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


bool checkRobots(robot *robots, int numberOfRobots) {

  for(int i = 0; i < numberOfRobots; i++) {
    if(robots[i].atTarget == false) {
      return false;
    }
  }
  return true;
}


int main(void) {

  int rows, columns, numberOfRobots;

  printf("Enter the number of rows: ");
  scanf("%i", &rows);
  printf("Enter the number of columns: ");
  scanf("%i", &columns);
  printf("Enter the number of robots: ");
  scanf("%i", &numberOfRobots);

  srand(time(NULL));
  int targetR = rand() % rows;
  int targetC = rand() % columns;

  pthread_t *robotThreads;
  robotThreads = (pthread_t *)malloc(numberOfRobots * sizeof(pthread_t));


  int **grid = createGrid(rows, columns, targetR, targetC);
  robot *robots = createRobots(numberOfRobots, rows, columns, grid, robotThreads);

  //initial print without any searchs
  printGrid(rows, columns, grid);

  bool targetFound = false;

  //initial look around
  for(int robotNum = 0; robotNum < numberOfRobots; robotNum++) {
    targetFound = robotSearchAround(&robots[robotNum]);
  }

  printGrid(rows, columns, grid);

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
  }

  //TARGET FOUND
  alert(robots, numberOfRobots, targetR, targetC);

  bool allRobotsAtTarget = checkRobots(robots, numberOfRobots);

  while(allRobotsAtTarget == false) {
    for(int robotNum = 0; robotNum < numberOfRobots; robotNum++) {
      moveRobotsToTarget(&robots[robotNum], numberOfRobots);
      printf("%d", robots[robotNum].atTarget);
    }
    printGrid(rows, columns, grid);
    allRobotsAtTarget = checkRobots(robots, numberOfRobots);
  }


  //print out statistics
  printf("\n");
  printf("Target surrounded. Number of moves:\n");
  for(int i = 0; i < numberOfRobots; i++) {

    printf("Robot Number %d: %d moves\n", robots[i].num, robots[i].numberOfMoves);    

  }



  for(int i = 0; i < numberOfRobots; i++) {
    pthread_join(robotThreads[i], NULL);
  }

}

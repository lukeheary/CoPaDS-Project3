// name: Luke Heary
// email: ldh4586@rit.edu
// file: main.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "main.h"


int **createGrid(int rows, int columns) {
  
  int **grid = (int **)calloc(rows, rows * sizeof(int));
  for(int i = 0; i < rows; i++) {
    grid[i] = (int *)calloc(columns, columns * sizeof(int));
  }

  srand(time(NULL));
  int randomR = rand() % rows;
  int randomC = rand() % columns;

  printf("\nTarget Coordinates: %d, %d \n\n", randomR, randomC);

  grid[randomR][randomC] = 1;

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


int main(void) {

  int rows, columns, numberOfRobots;


  printf("Enter the number of rows: ");
  scanf("%i", &rows);
  printf("Enter the number of columns: ");
  scanf("%i", &columns);
  printf("Enter the number of robots: ");
  scanf("%i", &numberOfRobots);

  int **grid = createGrid(rows, columns);
  pthread_t *robots = createRobots(numberOfRobots, rows, columns, grid);

  // start cycling through moves

  //for loop for move number
  for(int moveNum = 0; moveNum < 2; moveNum++) {

    //for loop for individual robot move
    for(int robotNum = 0; robotNum < numberOfRobots; robotNum++) {
      moveRobot((robot *)robots[robotNum]);
    }

    printGrid(rows, columns, grid);
  }



  for(int i = 0; i < numberOfRobots; i++) {
    pthread_join(robots[i], NULL);
  }

  

}

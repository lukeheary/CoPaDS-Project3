// name: Luke Heary
// date: 11/14/17
// file: robot.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include "main.h"

void robotSearchAround(int rows, int columns, int **grid, int currentR, int currentC, int robotNumber, int *cellsAround) { 
 
  int tl=0, tm=0, tr=0, ml=0, mr=0, bl=0, bm=0, br=0;
  int C = currentR;
  int R = currentC;
  bool bottomRow = true, topRow = true, rightColumn = true, leftColumn = true;

  // check to see if the robot is on the edge of the grid
  if(R + 1 == columns) { 	bottomRow = false;}
  if(R - 1 < 0) { 		topRow = false; }
  if(C + 1 == rows) { 		rightColumn = false; }
  if(C - 1 < 0) { 		leftColumn = false; }  

  // check around the robot to see if anything is near
  if(topRow) {
    if(leftColumn) { 
      tl = grid[C-1][R-1];				//top left 
      cellsAround[0] = tl;
      if(tl == 0) {grid[C-1][R-1] = 3;}
   }							
    ml = grid[C][R-1];					//mid left
    cellsAround[1] = ml;
    if(ml == 0) {grid[C][R-1] = 3; }    	 	
    if(rightColumn) { 
      bl = grid[C+1][R-1]; 				//bot left
      cellsAround[2] = bl;
      if(bl == 0) {grid[C+1][R-1] = 3; }						
    }							
  }

  if(leftColumn) { 
    tm = grid[C-1][R];					//top mid
    cellsAround[3] = tm;
    if(tm == 0) {grid[C-1][R] = 3; }	
   }							
  if(rightColumn) { 
    bm = grid[C+1][R]; 					//bot mid
    cellsAround[4] = bm;
    if(bm == 0) {grid[C+1][R] = 3; }
  } 	

  if(bottomRow) {
    if(leftColumn) { 
      tr = grid[C-1][R+1]; 				//top right
      cellsAround[5] = tr;
      if(tr == 0) {grid[C-1][R+1] = 3; }
    }							
    mr = grid[C][R+1];					//mid right
    cellsAround[6] = mr;
    if(mr == 0) {grid[C][R+1] = 3; }	 	  	
    if(rightColumn) { 
      br = grid[C+1][R+1]; 				//bot right
      cellsAround[7] = br;
      if(br == 0) {grid[C+1][R+1] = 3; }
    }								
  }

  //for (int i = 0; i < 8; i++) {
    //printf("%d", cellsAround[i]);
  //}

  //printf("\n%d: (%d, %d), %d %d %d %d %d %d %d %d\n", robotNumber, C, R, tl, ml, bl, tm, bm, tr, mr, br);
}


void moveRobot(robot *r) {

  

  int x = (*r).robotNumber;
  printf("cur robot: %d", x);
  
  return;
}



void * robotCommander(void *currentRobot) {
  robot *r = (robot *) currentRobot;
  robotSearchAround(r->rows, r->columns, r->grid, r->currentR, r->currentC, r->robotNumber, r->cellsAround);
  return r;
}


pthread_t * createRobots(int numberOfRobots, int rows, int columns, int **grid) {

  pthread_t *robots;
  robot *currentRobot;

  robots = (pthread_t *)malloc(numberOfRobots * sizeof(pthread_t));
  currentRobot = (robot *)malloc(numberOfRobots * sizeof(robot)*numberOfRobots);

  for(int i = 0; i < numberOfRobots; i++) {

    int randomR = rand() % rows;
    int randomC = rand() % columns;

    // check if a cell is already occupied
    while(grid[randomR][randomC] != 0) {
      randomR = rand() % rows;
      randomC = rand() % columns;
    }

    currentRobot[i].robotNumber = i;
    currentRobot[i].initialR = randomR;
    currentRobot[i].currentR = randomR;
    currentRobot[i].initialC = currentRobot[i].currentC = randomC;
    currentRobot[i].numberOfMoves = 0;
    currentRobot[i].rows = rows;
    currentRobot[i].columns = columns;
    currentRobot[i].grid = grid;
    currentRobot[i].cellsAround = (int*)calloc(8, sizeof(int));
   
    // place robot on the grid 
    grid[currentRobot[i].initialR][currentRobot[i].initialC] = 2;

    pthread_create(&robots[i], NULL, robotCommander, (void *)(currentRobot+i));
  }

  return robots;
}




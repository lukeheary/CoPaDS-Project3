// name: Luke Heary
// date: 11/14/17
// file: robot.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include "main.h"

void robotSearchAround(robot *r) { 

  int rows = r->rows;
  int columns = r->columns;
  int **grid = r->grid;
  int C = r->currentR;
  int R = r->currentC;
  int robotNumber = r->robotNumber;
  int *cellsAround = r->cellsAround;

  int tl=0, tm=0, tr=0, ml=0, mr=0, bl=0, bm=0, br=0;
  bool bottomRow = true, topRow = true, rightColumn = true, leftColumn = true;

  // check to see if the robot is on the edge of the grid
  if(R + 1 == columns) { 	bottomRow = false; bl = 4; bm = 4; br = 4;}
  if(R - 1 < 0) { 		topRow = false; tl = 4; tm = 4; tr = 4;}
  if(C + 1 == rows) { 		rightColumn = false; tr = 4; mr = 4; br = 4;}
  if(C - 1 < 0) { 		leftColumn = false; tl = 4; ml = 4; bl = 4;}  

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

  if(R + 1 == columns) { 	cellsAround[2] = 4; cellsAround[4] = 4; cellsAround[7] = 4;}
  if(R - 1 < 0) { 		cellsAround[0] = 4; cellsAround[3] = 4; cellsAround[5] = 4;}
  if(C + 1 == rows) { 		cellsAround[5] = 4; cellsAround[6] = 4; cellsAround[7] = 4;}
  if(C - 1 < 0) { 		cellsAround[0] = 4; cellsAround[1] = 4; cellsAround[2] = 4;}  

  r->cellsAround = cellsAround;
}

void alert(robot *robots, int numberOfRobots, int cell) {

  // take in the coords of target, loop through list of robots, fill in the target coords in each robot struct, have all robots travel to target 

  printf("\n\nTARGET FOUND, BROADCASTING TO ROBOTS\n\n");

  for(int i = 0; i < numberOfRobots; i++) {
    //&robots[i]->targetR = 
  }

  exit(0);
}


int **moveRobot(robot *r, robot *robots, int numberOfRobots) {

  //printf("cur robot: %d, numofmoves: %d", r->robotNumber, r->numberOfMoves);

  // set current robot cell to blank
  r->grid[r->currentR][r->currentC] = 0;
  
  int x = r->numberOfMoves % 4;

  if(x == 0 && r->cellsAround[6] != 4) {
    r->currentR++;		// right
  } else if (x == 1 && r->cellsAround[3] != 4) {
    r->currentC++;		// up
  } else if (x == 2 && r->cellsAround[1] != 4) {
    r->currentR--;		// left
  } else if (x == 3 && r->cellsAround[4] != 4) {
    r->currentC--;		// down;
  }

  r->grid[r->currentR][r->currentC] = 2;
  r->numberOfMoves++;

  for (int i = 0; i < 8; i++) {
    if(r->cellsAround[i] == 1) {
        int currentRobotR = r->currentR;
        int currentRobotC = r->currentC;

        switch(i)
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':

	alert(robots, numberOfRobots, i);
    }
    printf("%d", r->cellsAround[i]);
  } printf(", ");

  return r->grid;
}



void * robotCommander(void *currentRobot) {
  robot *r = (robot *) currentRobot;
  robotSearchAround(r);
  return r;
}


robot * createRobots(int numberOfRobots, int rows, int columns, int **grid) {

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

  return currentRobot;
}




// name: Luke Heary
// date: 11/14/17
// file: robot.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include "main.h"


void alert(robot *robots, int numberOfRobots, int targetR, int targetC) {

  // take in the coords of target, loop through list of robots, fill in the target coords in each robot struct, have all robots travel to target 

  printf("\n\nTARGET FOUND, BROADCASTING TO ROBOTS\n\n");

  for(int i = 0; i < numberOfRobots; i++) {
    robots[i].targetR = targetR;
    robots[i].targetC = targetC;
    //printf("(%d, %d)", robots[i].targetR, robots[i].targetC); 
  }
  exit(0);
}


void moveRobotsToTarget(&robots[robotNum], robots, numberOfRobots) {

}






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

  //printf("current: (%d, %d)", r->currentR, r->currentC);

  r->cellsAround = cellsAround;
}

bool moveRobot(robot *r, robot *robots, int numberOfRobots) {

  // set current robot cell to blank
  r->grid[r->currentR][r->currentC] = 0;

  // IF the robot is on a corner, make them move one cell inward to be more efficient
  if(r->currentR == r->rows - 1 /** || r->grid[r->currentR - 1][r->currentC] == 3 **/) {
    r->currentR--;
    r->robotNumber++;
  } else if (r->currentR == 0) {
    r->currentR++;
    r->robotNumber++;
  } else if (r->currentC == r->columns - 1) {
    r->currentC--;
    r->robotNumber++;
  } else if (r->currentC == 0) {
    r->currentC++;
    r->robotNumber++;
  } else {

    int x = r->robotNumber % 4;

    if(x == 0) {
      r->currentC++;
    } else if(x == 2) {
      r->currentC--;
    } else if(x == 1) { 
      r->currentR++;
    } else if(x == 3) {
      r->currentR--;
    }
  }

  r->grid[r->currentR][r->currentC] = 2;
  r->numberOfMoves++;

  for (int i = 0; i < 8; i++) {
    if(r->cellsAround[i] == 1) {
        int targetR = r->currentR;
        int targetC = r->currentC;

        switch(i) {
          case '0':
            targetR--; targetC--; break;
          case '1':
            targetC--; break;
          case '2':
            targetR++; targetC--; break;
          case '3':
            targetR--; break;
          case '4':
            targetR++; break;
          case '5':
            targetR--; targetC++; break;
          case '6':
            targetC++; break;
          case '7':
            targetR++; targetC++; break;
          }
        return true;
	alert(robots, numberOfRobots, targetR, targetC);
    }
    //printf("%d", r->cellsAround[i]);
  } //printf(", ");

  //return r->grid;
  return false;
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
    currentRobot[i].btml = false;
   
    // place robot on the grid 
    grid[currentRobot[i].initialR][currentRobot[i].initialC] = 2;

    pthread_create(&robots[i], NULL, robotCommander, (void *)(currentRobot+i));
  }

  return currentRobot;
}




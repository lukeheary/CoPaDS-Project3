// name: Luke Heary
// date: 11/14/17
// file: robot.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include "main.h"

// alerts the robots to where the target is
//
// @param *robots		the list of robots
// @param numberOfRobots	the number of robots
// @param targetR		the row of the target
// @param targetC		the col of the target
// @return none
void alert(robot *robots, int numberOfRobots, int targetR, int targetC) {

  printf("\n\nTARGET FOUND, BROADCASTING TO ROBOTS\n\n");

  for(int i = 0; i < numberOfRobots; i++) {
    robots[i].targetR = targetR;
    robots[i].targetC = targetC;
  }
}

// moves all of the robots towards the target
//
// @param *r			pointer to current robot
// @param numberOfRobots	the number of robots
// @return none
void moveRobotsToTarget(robot *r, int numberOfRobots) { 

  if(r->atTarget == false) {
    
    r->grid[r->currentR][r->currentC] = 3;

    // move the robot towards the target
    if(r->currentR > r->targetR) { 
      if(r->grid[r->currentR - 1][r->currentC] == 2) {
        r->touchingRobot = true;
      } else if(r->grid[r->currentR - 1][r->currentC] == 1) {
        r->atTarget = true;
      } else {
        r->currentR--;
      }

    } else if(r->currentR < r->targetR) {
      if(r->grid[r->currentR + 1][r->currentC] == 2) {
        r->touchingRobot = true;
      } else if(r->grid[r->currentR + 1][r->currentC] == 1) {
        r->atTarget = true;
      } else {
        r->currentR++;
      }

    } else if(r->currentC > r->targetC) {
      if(r->grid[r->currentR][r->currentC - 1] == 2) {
        r->touchingRobot = true;
      } else if(r->grid[r->currentR][r->currentC - 1] == 1) {
        r->atTarget = true;
      } else {
        r->currentC--;
      }

    } else if(r->currentC < r->targetC) {

      if(r->grid[r->currentR][r->currentC + 1] == 2) {
        r->touchingRobot = true;
      } else if(r->grid[r->currentR][r->currentC + 1] == 1) {
        r->atTarget = true;
      } else {
        r->currentC++;
      }
    }

  if(r->touchingRobot == true && r->atTarget == false) {

      if(r->currentC > r->targetC && r->atTarget == false && r->grid[r->currentC - 1][r->currentR - 1] != 2 && r->grid[r->currentC - 1][r->currentR - 1] != 1) {
        r->currentR--;
        r->currentC--;
        r->atTarget = robotSearchAround(r);
        //while(r->grid[r->currentC - 1][r->currentR] != 2) {
          //r->currentC--;
         // r->atTarget = robotSearchAround(r);
        }
      } else if(r->currentC < r->targetC && r->atTarget == false && r->grid[r->currentC + 1][r->currentR - 1] != 2 && r->grid[r->currentC + 1][r->currentR - 1] != 1) {
        r->currentR--;
        r->currentC++;
        r->atTarget = robotSearchAround(r);

      } else if(r->currentC > r->targetC && r->atTarget == false && r->grid[r->currentC - 1][r->currentR + 1] != 2 && r->grid[r->currentC - 1][r->currentR + 1] != 1) {
        r->currentR++;
        r->currentC--;
        r->atTarget = robotSearchAround(r);

      } else if(r->currentC > r->targetC && r->atTarget == false && r->grid[r->currentC + 1][r->currentR + 1] != 2 && r->grid[r->currentC + 1][r->currentR + 1] != 1) {
        r->currentR++;
        r->currentC++;
        r->atTarget = robotSearchAround(r);
      }
    }
  r->grid[r->currentR][r->currentC] = 2;
  r->numberOfMoves++;
  r->atTarget = robotSearchAround(r);
}

// searches the eight cells around a robot
//
// @param *r		pointer to current robot
// @return 		true if target found, false if no target
bool robotSearchAround(robot *r) { 

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

  r->cellsAround = cellsAround;

   for (int i = 0; i < 8; i++) {
    if(r->cellsAround[i] == 1) {
      r->atTarget = true;
      return true;
    } else if (r->cellsAround[i] == 2) {
      r->touchingRobot = true;
    }
    //printf("%d", r->cellsAround[i]);
  } //printf(", ");
  return false;
}

// moves the robot
//
// @param *r			pointer to current robot
// @param numberOfRobots	the number of robots
void moveRobot(robot *r, int numberOfRobots) {

  // set current robot cell to blank
  r->grid[r->currentR][r->currentC] = 0;

  // WORKS, INEFFICIENT

  int x = rand() % 8;
  if(x == 2) {
    r->robotNumber++;
  }

  // IF the robot is on a corner, make them move one cell inward to be more efficient
  if(r->currentR == r->rows - 1) {
    if(r->grid[r->currentR - 1][r->currentC] != 2) {
      r->currentR--;
    } else {r->currentR++;}; 

    r->robotNumber++;
  } else if (r->currentR == 0) {
    if(r->grid[r->currentR + 1][r->currentC] != 2) {
      r->currentR++;
    } else {r->currentR--;};

    r->robotNumber++;;
  } else if (r->currentC == r->columns - 1) {
    if(r->grid[r->currentR][r->currentC - 1] != 2) {
      r->currentC--;
    } else {r->currentC++;};

    r->robotNumber++;
  } else if (r->currentC == 0) {
    if(r->grid[r->currentR][r->currentC + 1] != 2) {
      r->currentC++;
    } else {r->currentC--;};

    r->robotNumber++;
  } else {

    int x = r->robotNumber % 4;

    if(x == 0) {
      r->currentC++;
    } else if(x == 1) {
      r->currentC--;
    } else if(x == 2) { 
      r->currentR++;
    } else if(x == 3) {
      r->currentR--;
    }
  } 

  r->grid[r->currentR][r->currentC] = 2;
  r->numberOfMoves++;
}


// thread helper function
//
// @param *currentRobot		the current robot
// @return r			the robot
void * robotCommander(void *currentRobot) {
  robot *r = (robot *) currentRobot;
  //robotSearchAround(r);
  return r;
}

// creates and fills in information for each robot
//
// @param numberOfRobots	the number of robots
// @param rows			number of rows in grid
// @param columns		number of cols in grid
// @param **grid		pointer to grid
// @param *robots		list of threads
// return currentRobot		list of robots
robot * createRobots(int numberOfRobots, int rows, int columns, int **grid, pthread_t *robots) {

  robot *currentRobot;
  //pthread_t *robots;

  //robots = (pthread_t *)malloc(numberOfRobots * sizeof(pthread_t));
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
    currentRobot[i].num = i;
    currentRobot[i].touchingRobot = false;
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

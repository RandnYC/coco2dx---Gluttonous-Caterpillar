#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "degreeHandle.h"
#include <cmath>
#include <algorithm>
#include <queue>
#include <ctime>

#define PI 3.1415926
#define E 2.7182818
//Snake Action Parameter
#define MAX_ROT 45.0
#define SPEED 5.0
#define SPEED_ROT 1.5
#define RADIUS 25.0
#define MOVE_DELY 0.2
#define SCALE 1.05
#define DEFAULT_BODY_SIZE 5

//for Robot Evalue Parameter
#define COEF_WALL1 -0.65
#define COEF_WALL2 -0.05
#define COEF_ME -1.2
#define CONST_VALUE 10.0
#define CONST_SAFE 4.0
#define CONST_WARNING 6.2
#define CONST_DANGEROUS 7.2

#define INF 0x7fffffff
#define EPS 1e-3

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
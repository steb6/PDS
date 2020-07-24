#pragma once

#include <unistd.h> //usleep
#include <cstdlib> // atoi
#include <chrono>
#include <iostream>
#include <cfloat>
#include <algorithm>
#include <set>
#include <atomic>
#include <thread>
#include <vector>
#include <cmath>  // sqrt
#include <math.h> // pow
#include <time.h> // time
#include <stdio.h> // this and following for kbhit()
#include <termios.h> //kbit
#include <fcntl.h> //khbit
#include <mutex>
#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <tuple>
#include <random>
using namespace ff;

#include "MyRandom.h"
#include "utilities.h"
#include "City.h"
#include "Population.h"
#include "utimer.cpp"


#ifdef GRAPH
#include "Draw.h"
#endif

#include "GA.h"

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
#include <ff/parallel_for.hpp>
#include <cmath>  // sqrt
#include <math.h> // pow
#include <time.h> // time
#include <stdio.h> // this and following for kbhit()
#include <termios.h> //kbit
#include <fcntl.h> //khbit
#include <mutex>

#include "utilities.h"
#include "City.h"
#include "Population.h"

#ifdef GRAPH
#include "Draw.h"
#endif

#include "GA.h"

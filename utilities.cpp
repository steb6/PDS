#include <stdio.h> // this and following for kbhit()
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "utilities.h"
#include <cstdlib> // rand
#include <iostream> // cout

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}


int pick_candidate(std::vector<double> probabilities){
    double r = ((double) rand() / (RAND_MAX));
    int i=0;
    while(r>0){
	r -= probabilities[i];
	i++;
    }
    i--;

    return i;
}

void log(int i, int pop_size, int nw, int time, int n_nodes){
    std::cout << "#" << i << "; pop_size: " << pop_size << "; nw: " << nw << "; time: " << (double)time/1000000 << "; #nodes: " << n_nodes << "; thr: " << ((double)i/(double)time)*1000000 << "\r";
    std::cout.flush();
/*float progress = 0.0; //TODO progressbar
while (progress < 1.0) {
    int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();

    progress += 0.000001; // for demonstration only
}
std::cout << std::endl;*/
}

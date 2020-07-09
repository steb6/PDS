#include "dependencies.h"

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

void log(int i, int pop_size, int nw, long time, int n_nodes){
    std::cout << "#" << i << "; pop_size: " << pop_size << "; nw: " << nw << "; #nodes: " << n_nodes << "; time: " << (double)time/1000000 << "; thr: " << ((double)i/(double)time)*1000000 << "\r";
    std::cout.flush();
}

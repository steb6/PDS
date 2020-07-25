#include "dependencies.h"
#include <graphics.h>

Draw::Draw(int r, int n){
    gd = DETECT;
    initgraph(&gd,&gm,NULL);
    setcolor(2);
    radius = r;
    n_nodes = n;
}

void Draw::print_city(std::vector<int> x, std::vector<int> y){
    for(int i=0; i<n_nodes; i++){
	circle(x[i], y[i], radius);
	char label[256]; // to convert label from int to char+NULL
	sprintf(label, "%d", i);
	outtextxy(x[i]-radius/2, y[i]-radius/2, label);
    }
}

void Draw::print_best_one(std::vector<int> best_path, std::vector<int> x, std::vector<int> y){
    for(int i=0; i<n_nodes-1; i++){
        line(x[best_path[i]], y[best_path[i]], x[best_path[i+1]], y[best_path[i+1]]);
    }
}

void Draw::clear(){
    cleardevice();
}

void Draw::close(){
    closegraph();
}

#include "dependencies.h"
#include <graphics.h>

Draw::Draw(int r, int n){
    gd = DETECT;
    initgraph(&gd,&gm,NULL);
    setcolor(2);
    radius = r;
    n_nodes = n;
}

void Draw::print_city(std::vector<int> x, std::vector<int> y, std::vector<std::vector<bool>> adj){
    for(int i=0; i<n_nodes; i++){
	circle(x[i], y[i], radius);
	char label[256]; // to convert label from int to char+NULL
	sprintf(label, "%d", i);
	outtextxy(x[i]-radius/2, y[i]-radius/2, label);
    }
    // print roads
    setcolor(3);
    setlinestyle(1, 0, 1);
    for(int i=0; i<n_nodes; i++){
        for(int j=0; j<n_nodes; j++){
	    if(adj[i][j])
		line(x[i], y[i], x[j], y[j]);
	}
    }
    setcolor(2);
    setlinestyle(0, 0, 3);
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

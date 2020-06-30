#include "Draw.h"
#include <graphics.h>

Draw::Draw(int r, int n, int t){
    gd = DETECT;
    initgraph(&gd,&gm,NULL);
    setcolor(2);
    radius = r;
    n_nodes = n;
    top_bar = t;
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

void Draw::print_info(int i, int p, double best_length, int nw, double time, int n_nodes){

    char value[64];
    char label1[] = "Iterations: ";

    // print n_nodes
    char label6[] = "n_nodes: ";
    outtextxy(410, 2+(top_bar/2), label6);
    sprintf(value, "%d", n_nodes);
    outtextxy(560, 2+(top_bar/2), value);
    // print pop_size
    char label2[] = "Population size: ";
    outtextxy(410, 2, label2);
    sprintf(value, "%d", p);
    outtextxy(560, 2, value);
    // print nw
    char label4[] = "nw: ";
    outtextxy(220, 2+(top_bar/2), label4);
    sprintf(value, "%d", (int)nw);
    outtextxy(320, 2+(top_bar/2), value);
    // print best length
    char label3[] = "Best length: ";
    outtextxy(30, 2, label3);
    sprintf(value, "%d", (int)best_length);
    outtextxy(130, 2, value);
    // print iterations
    outtextxy(220, 2, label1);
    sprintf(value, "%d", i);
    outtextxy(320, 2, value);
    // print time
    char label5[] = "time: ";
    outtextxy(30, 2+(top_bar/2), label5);
    sprintf(value, "%.2lf", time/1000000);
    outtextxy(130, 2+(top_bar/2), value);

}

void Draw::close(){
    closegraph();
}

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include "SimpleGraph.h"

const double Pi = 3.14159265358979323;
using std::cout;	using std::endl; using std::cin;

int n;
void Welcome();
void Read();
void Init();
// Main method
int main() {
    Welcome();
    Read();
    Init();

    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}
void Read() {
    cout << "Please type in the graph in following rules:" << endl;
    cout << "The first line gives the number of nodes." << endl;
    cout << "The following each line contains two numbers denoting which nodes are connected by edges." << endl;
    cin >> n;

}
void Init() {

    SimpleGraph graph;
    for(int i = 1; i <= n; i++) {
     //   nodes.push_back();
        graph.nodes[i-1].x = cos(2*Pi*i/n);
        graph.nodes[i-1].y = sin(2*Pi*i/n);
    }
    InitGraphVisualizer();
    DrawGraph();
}

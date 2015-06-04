// **************************************************************************
//  File       [main.cpp]
//  Author     [Kai-Wei Chang]
//  Synopsis   [The main program of 2015 Spring Algorithm Final Project]
// **************************************************************************

#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include "graph.h"

#define INF 2147483647
#define NEG_INF -2147483648

using namespace std;
size_t myStrGetTok(const string&, string&, size_t = 0, const char = ',');

void help_message() {
    cout << "usage: DPT_balance_color ($input_file_name) ($input_file_name).out" << endl;
}

int main(int argc, char* argv[])
{
    if(argc != 3) {
       help_message();
       return 0;
    }

    //////////// read the input file /////////////
    
    fstream input;
    input.open(argv[1]);
    if (!input.is_open()) {
       cout << "Error: File is not open!!" << endl;
       return 0;
    }
    
    Graph graph;
    string buf;
    size_t pos;
    
    // parse alpha
    getline(input, buf);
    pos = buf.find_first_of("=");
    graph.alpha = atoi(buf.substr(pos+1).c_str());
    
    // parse beta
    getline(input, buf);
    pos = buf.find_first_of("=");
    graph.beta = atoi(buf.substr(pos+1).c_str());
    
    // parse omega
    getline(input, buf);
    pos = buf.find_first_of("=");
    graph.omega = atoi(buf.substr(pos+1).c_str());
    
    // parse nodes
    int coordinate[4], count = 0, id = 0;
    int x1 = INF, y1 = INF, x2 = NEG_INF, y2 = NEG_INF;    
    string tok;
    while (getline(input, buf)) {
       pos = 0;
       count = 0;
       ++id;
       while (pos != string::npos) {
          pos = myStrGetTok(buf, tok, pos);
          coordinate[count] = atoi(tok.c_str());
          ++count;
       }
       Node* newNode = new Node(id, coordinate);
       if (coordinate[0] < x1)
          x1 = coordinate[0];
       if (coordinate[1] < y1)
          y1 = coordinate[1];
       if (coordinate[2] > x2)
          x2 = coordinate[2];
       if (coordinate[3] > y2)
          y2 = coordinate[3];    
       graph.nodesMap[id] = newNode;
       graph.nodes.push_back(newNode);
    } 
    input.close();
    
    graph.x1 = x1;
    graph.y1 = y1;
    graph.x2 = x2;
    graph.y2 = y2;  
    
    // add edges
    graph.addEdgesInY();
    graph.addEdgesInX();
    
    //////////// find the solution ////
    



    //////////// write the output file ///////////
        
    fstream output;
    output.open(argv[2], ios::out);
    if (!output) {
       cout << "Error: File is not open!!" << endl;
       return 0;
    }
    
    output << "// used for debugging graph" << endl 
           << "graph {" << endl;   
    for (size_t i=0; i<graph.edges.size(); ++i)
        output << "v" << graph.edges[i]->node[0]->id << " -- v" 
             << graph.edges[i]->node[1]->id << ";" << endl;
    output << "}";

    output.close();
    return 0;
}


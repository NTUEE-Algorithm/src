// **************************************************************************
//  File       [main.cpp]
//  Author     [Kai-Wei Chang]
//  Synopsis   [The main program of 2015 Spring Algorithm Final Project]
// **************************************************************************

#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits> 
#include "graph.h"
#include "map.h"

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
        graph.nodesMap[id] = newNode;
        graph.nodes.push_back(newNode);
    } 
    input.close();

    // add edges
    graph.addEdgesInY();
    graph.addEdgesInX();
    
    //////////// find the solution ////
    
    Map map(&graph);
    map.makeGroup();   // make group and color    
    map.InitXY();
    map.CreatWindow();
    map.linkGW();
    map.BuildAllColor();
    map.InitEffect();
    map.gdColor();

    //////////// write the output file ///////////

        
    fstream output;
    output.open(argv[2], ios::out);
    if (!output) {
        cout << "Error: File is not open!!" << endl;
        return 0;
    }
    map.printFile(output);    
/*    
    output << "// used for debugging graph" << endl 
           << "graph {" << endl;   
    for (size_t i=0; i<graph.edges.size(); ++i)
        output << "v" << graph.edges[i]->node[0]->id << " -- v" 
               << graph.edges[i]->node[1]->id << ";" << endl;
    output << "}";
*/
    output.close();
    return 0;
}

#ifndef MAP_H
#define MAP_H

#include <vector>

using namespace std;

class Window;
class Graph;

class Group {
   public:
      Group(Node* h, int* pos);//do not handle anything about window
      
   private:
      vector<Window*> windows;
      double effect;
      // pos
      //left down
      int x1;
      int y1;
      //right top
      int x2;
      int y2;
      
      Node* head;
      vector<Node *> nodes;
};

class Window {
   public:
      Window( int omega, int x, int y );
      int x, y;
      int OMEGA;
      vector<Group*> groups;

};

class Map {
   public:
      Map(Graph* g) { graph = g;}
      ~Map();
      void CreatWindow( int& OMEGA, int& X1, int& X2, int& Y1, int& Y2 );
      void makeGroup();
      
   private:
      Graph* graph;
      vector<Group*> groups;
      Window** windows;
};



#endif

#ifndef MAP_H
#define MAP_H

#include <vector>

using namespace std;

class Window;
class Graph;
class Node;

class Group {
   public:
      Group(Node* h, int* pos);//do not handle anything about window
      bool isGref();
      void setToGref();
		void setGref();
      
      Node* head;
      vector<Node *> nodes;
      Window*** gwindows;
      double effect;
      // pos
      //left down
      int x1;
      int y1;
      //right top
      int x2;
      int y2;
      int ref;
		static int gref;
};

class Window {
   public:
      Window( int omega, int x, int y );
      int WX, WY;
      int OMEGA;
      vector<Group*> wgroups;
      vector<int> color;
      void BuildColor();
};

class Map {
   public:
      Map(Graph* g) { graph = g;}
      ~Map();
      int OMEGA;
      void CreatWindow( int& OMEGA, int& X1, int& X2, int& Y1, int& Y2 );
      void makeGroup();
      void linkGW();
      void sortByeffect();
      void gdColor();

   private:
      Graph* graph;
      vector<Group*> groups;
      Window*** windows;
};

int MinMax( vector<int>& v );

#endif

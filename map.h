#ifndef MAP_H
#define MAP_H

#include <vector>

using namespace std;

class Window;
class Graph;
class Node;
class Donegroup;

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
      
      int numberofWindow(Donegroup& dg, Group* g); 
      void tryBest(Donegroup& dg, Group* g);
      void markAll(Donegroup& dg, Group* g);
      void sortByeffect();
      void gdColor();
      void getWindowNumber(Donegroup& dg, Group* g, int* result);

   private:
      Graph* graph;
      vector<Group*> groups;
      Window*** windows;
};

class Donegroup{
   public:
	Donegroup(Group* g):x1(g->x1), y1(g->y1), x2(g->x2), y2(g->y2){}
	void update(Group* g){
	   if(x1>g->x1) x1=g->x1;
	   if(y1>g->y1) y1=g->y1;
		if(x2<g->x2) x2=g->x2;
		if(y2<g->y2) y2=g->y2;
	}
	//left down
   int x1;
   int y1;
   //right top
   int x2;
   int y2;
};

int MinMax( vector<int>& v );

#endif

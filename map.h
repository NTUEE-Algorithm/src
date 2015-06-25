#ifndef MAP_H
#define MAP_H

#include <vector>
#include <fstream>

using namespace std;

class Window;
class Graph;
class Node;
class Map;
class Donegroup;

class Group {
   public:
      Group(Node* h);//do not handle anything about window
      void setPos(int* pos);
      bool isGref();
      void setToGref();
      void setGref();
      bool isGref2();
      void setToGref2();
      void setGref2();
      
      void reverse();
      
      Node* head;
      vector<Node *> nodes;
      double effect;
      // pos
      //left down
      int x1;
      int y1;
      //right top
      int x2;
      int y2;
      int ref;
      int ref2;
      int color1;
      int color2;
      static int gref;
      static int gref2;
      
      bool rev;
};

class Window {
   public:
      Window( int omega, int x, int y );
      int WX, WY;
      int OMEGA;
      vector<Group*> wgroups;
      vector<int> color;
      vector<int> color1;
      vector<int> color2;
      void BuildColor();
      
      bool isGref();
      void setToGref();
      void setGref();
      int ref;
      static int gref;
      double print(fstream& output, Map* map);
};

class Map {
   public:
      Map(Graph* g);
      ~Map();
      int OMEGA;
      int X1;
      int Y1;
      int WNAX;
      int WNAY;
      int X2;   //boundary
      int Y2;   //boundary
      void CreatWindow();
      void makeGroup();
      void linkGW();
      bool CheckSharing( Group* g, Window* w );
      int  numberofGroup(Donegroup& dg, Group* g);
      void tryBest(Donegroup& dg, Group* g);
      void sortByeffect();
      void gdColor();
      void getWindowNumber(Donegroup& dg, Group* g, int* result);
      void InitEffect();
      void InitXY();
      void BuildAllColor();
      int  MinMax( vector<int>& v, int& skip );
      double printFile(fstream& output);
      void optSolver();
      void justColor();

   private:
      Graph* graph;
      vector<Group*> groups;
      vector<Group*> ngroups;
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

#endif

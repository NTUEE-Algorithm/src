#include "map.h"

Group::Group(Node* h, int* pos)
{
    head = h;
    effect = 0;
    x1 = pos[0];
    y1 = pos[1];
    x2 = pos[2];
    y2 = pos[3];
};

Window::Window( int omega, int x1, int y1 )
{
    x=x1;
    y=y1;
    OMEGA=omega;
};

void CreatWindow( int& OMEGA, int& X1, int& X2, int& Y1, int& Y2 ){
    
    int width;
    int height;
    int WNAX;  //  window numbers along x without the last one
    int WNAY;  //  window numbers along y without the last one
    int xpin;
    int ypin;

    width=X2-X1;
    height=Y2-Y1;
    WNAX=width/OMEGA;
    WNAY=height/OMEGA;
    xpin=X1;
    ypin=Y1;
    Window*** win = new Window** [WNAY];
    for( int i=0 ; i<WNAY ; i++ ){
        win[i] = new Window* [WNAX];
    }

    for( int i=0 ; i<WNAX ; i++ ){
        for( int j=0 ; j<WNAY ; j++ ){
        	win[i][j] = new Window ( OMEGA, xpin, ypin );
        	ypin=ypin+OMEGA;
        }
        xpin=xpin+OMEGA;
    }

    xpin=X1;
    ypin=Y2-OMEGA;
    for( int i=0 ; i<WNAX ; i++ ){
        win[i][WNAY] = new Window( OMEGA, xpin, ypin );
        xpin=xpin+OMEGA;
    }

    xpin=X2-OMEGA;
    ypin=Y1;
    for( int j=0 ; j<WNAY ; j++ ){
        win[WNAX][j] = new Window( OMEGA, xpin, ypin );
        ypin=ypin+OMEGA;
    }
}

Map::~Map()
{
   for (size_t i=0; i<groups.size(); ++i)
      delete groups[i];
}

void Map::makeGroup()
{
   int pos[4];
   size_t n = graph->getNumofNode();
   graph->nodes[0]->setToGref();
   for(size_t i=0;i<n;++i)
   {
      Node* h = graph->nodes[i];
      if(!h->isGref();)
      {
          if(graph->coloring(h,pos))
          {
             Group* temp = new Group(h,pos);
             groups.push_back(temp);
             graph->setGroup(h);
          }else graph->markAll(h);
      }
   }
}

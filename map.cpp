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

Window::Window()
{

};

Map::~Map()
{
   for (size_t i=0; i<groups.size(); ++i)
      delete groups[i];
}

void Map::makeGroup()
{
   int pos[4];
   size_t n = graph->getNumofNode();
   graph->nodes[0]->setGref();
   graph->nodes[0]->setGref2()
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

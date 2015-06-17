#include "map.h"
#include "graph.h"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;
int Group::gref = 0;
int Group::gref2 = 0;

Group::Group(Node* h, int* pos)
{
    head = h;
    effect = 0;
    x1 = pos[0];
    y1 = pos[1];
    x2 = pos[2];
    y2 = pos[3];
    ref = gref;
    ref2 = gref2;
};

bool Group::isGref()
{
    return (ref == gref);
}

void Group::setToGref()
{
    ref = gref;
}

void Group::setGref()
{
    gref++;
}

bool Group::isGref2()
{
    return (ref2 == gref2);
}

void Group::setToGref2()
{
    ref2 = gref2;
}

void Group::setGref2()
{
    gref2++;
}

Window::Window( int omega, int x1, int y1 )
{
    WX=x1;
    WY=y1;
    OMEGA=omega;
};

void Window::BuildColor(){
    int effx1;
    int effx2;
    int effy1;
    int effy2;
    int area=0;
    int sum=0;
    size_t length_g=wgroups.size();
    for( int i=0 ; i<length_g ; i++ ){
        size_t length_n=wgroups[i]->nodes.size();
        for( int j=0 ; j<length_n ; j++ ){
              if( wgroups[i]->nodes[j]->x1 > WX+OMEGA )
                  continue;
              if( wgroups[i]->nodes[j]->x2 < WX )
                  continue;
              if( wgroups[i]->nodes[j]->y1 > WY+OMEGA )
                  continue;
              if( wgroups[i]->nodes[j]->y2 < WY )
                  continue;
              if( wgroups[i]->nodes[j]->x1 < WX )
                  effx1=WX;
              if( wgroups[i]->nodes[j]->x2 > WX+OMEGA )
                  effx2=WX+OMEGA;
              if( wgroups[i]->nodes[j]->y1 < WY )
                  effy1=WY;
              if( wgroups[i]->nodes[j]->y2 > WY+OMEGA )
                  effy2=WY+OMEGA;
              
              area=(effx2-effx1)*(effy2-effy1);

              if( wgroups[i]->nodes[j]->color )
                  sum=sum+area;
              else
                  sum=sum-area;
        }
    }
}

void Map::CreatWindow( int& OMEGA, int& X1, int& X2, int& Y1, int& Y2 ){
    
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

    xpin=X2-OMEGA;
    ypin=Y2-OMEGA;
    win[WNAX][WNAY] = new Window( OMEGA, xpin, ypin );
    
    windows = win;
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
   graph->nodes[0]->setGref();
   graph->nodes[0]->setGref2();
   for(size_t i=0;i<n;++i)
   {
      Node* h = graph->nodes[i];
      if(!h->isGref())
      {
          if(graph->coloring(h,pos))
          {
             Group* temp = new Group(h,pos);
             groups.push_back(temp);
             graph->setGroup(h, temp);
          }else graph->markAll(h);
      }
   }
}

int MinMax( vector<int>& v ){
    size_t pow = v.size();
    int MinCompare=100000;
    int MaxCompare=0;
    int counter=1;
    int temp=0;
    int sum=0;
    for( int i=0 ; i<pow-1 ; i++ ){
        counter=counter*2;
    }
    for( int i=0 ; i<counter ; i++ ){
        temp=i;
        sum=0;
        for( int j=0 ; j<pow ; j++ ){
            if(temp%2)
                sum=sum+v[j];
            else
                sum=sum-v[j];
            temp=temp/2;
        }
        cout << sum << endl;
        if(sum<0)
            sum=-sum;
        if(sum<MinCompare)
            MinCompare=sum;
        if(sum>MaxCompare)
            MaxCompare=sum;
    }
    return MaxCompare-MinCompare;
}

void Map::linkGW(){
    int m;
    int n;
    int o;
    int p;
    size_t Glength=groups.size();
    for( int h=0 ; h<Glength ; h++ ){
        m=(groups[h]->x1)/OMEGA;
        n=(groups[h]->y1)/OMEGA;
        o=(groups[h]->x2)/OMEGA+1;
        p=(groups[h]->y2)/OMEGA+1;
        for( int i=m ; i<=o ; i++ ){
            for( int j=n ; j<=p ; j++ ){
                windows[i][j]->wgroups.push_back(groups[h]);
            }
        }
    }
}

int Map::numberofWindow(Donegroup& dg, Group* g){
    return 0;
}

void Map::tryBest(Donegroup& dg, Group* g){
	
}

void Map::markAll(Donegroup& dg, Group* g){
	
}

bool effectCompare(Group* g1, Group* g2){
    return ((g1->effect)>(g2->effect));
}

void Map::sortByeffect(){
    sort(groups.begin(),groups.end(),effectCompare);
}

void Map::gdColor(){
    sortByeffect();
    groups[0]->setGref();
	
    int maxnum=10;
    int range=10;
	
    size_t n=groups.size();
	
    Donegroup dg(groups[0]);

    for(size_t i=1;i<n;){
        size_t j=i;
	size_t jtemp=i;
	int maxnumtemp = numeric_limits<int>::max();
	bool notexist=true;
		
	for(;j<n&&j<i+n/range;++j){
	    if(groups[j]->isGref()){
                notexist=false;
		int num=numberofWindow(dg, groups[j]);
	        if(num<maxnum) break;
	        else if(num<maxnumtemp){
	            jtemp=j;
	            maxnumtemp=num;
		}
	        if(j==n-1){
		    j=jtemp;
		    maxnum=maxnumtemp;
		    break;
	        }
	    }
	}
	if(notexist){
            i=(n>i+n/range)?i+n/range :n;
	}else{
            if(i==j) ++i;
	    tryBest(dg, groups[j]);
	    markAll(dg, groups[j]);
	    dg.update(groups[j]);
	}
    }
}

void Map::getWindowNumber(Donegroup& dg, Group* g, int* result){
	
}

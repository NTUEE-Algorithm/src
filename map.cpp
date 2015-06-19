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
    rev = false;
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

void Group::reverse(){
    size_t n=nodes.size();
    for(size_t i=0;i<n;++i){
        if(nodes[i]->color==0)nodes[i]->color=1;
	else nodes[i]->color=0;
    }
}

int Window::gref = 0;
Window::Window( int omega, int x1, int y1 )
{
    WX=x1;
    WY=y1;
    OMEGA=omega;
    ref = gref;
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

bool Window::isGref()
{
    return (ref == gref);
}

void Window::setToGref()
{
    ref = gref;
}

void Window::setGref()
{
    gref++;
}

void Map::CreatWindow( int& X1, int& X2, int& Y1, int& Y2 ){

    OMEGA = graph->omega;
    int width = X2-X1;
    int height = Y2-Y1;
    int WNAX = width/OMEGA;    //  window numbers along x without the last one
    int WNAY = height/OMEGA;   //  window numbers along y without the last one
    int xpin = X1;
    int ypin = Y1;
    Window*** win;
    
    if( width%OMEGA!=0 && height%OMEGA!=0 ){
        win = new Window** [WNAX+1];
        for( int i=0 ; i<WNAX+1 ; i++ )
            win[i] = new Window* [WNAY+1];
    }
    else if( width%OMEGA==0 && height%OMEGA!=0 ){
        win = new Window** [WNAX];
        for( int i=0 ; i<WNAX ; i++ )
            win[i] = new Window* [WNAY+1];
    }
    else if( width%OMEGA!=0 && height%OMEGA==0 ){
        win = new Window** [WNAX+1];
        for( int i=0 ; i<WNAX+1 ; i++ )
            win[i] = new Window* [WNAY];
    }
    else if( width%OMEGA==0 && height%OMEGA==0 ){
        win = new Window** [WNAX];
        for( int i=0 ; i<WNAX ; i++ )
            win[i] = new Window* [WNAY];
    }
    

    for( int i=0 ; i<WNAX ; i++ ){
        for( int j=0 ; j<WNAY ; j++ ){
            win[i][j] = new Window ( OMEGA, xpin, ypin );
            ypin=ypin+OMEGA;
        }
        xpin=xpin+OMEGA;
    }

    if( height%OMEGA!=0 ){
        xpin=X1;
        ypin=Y2-OMEGA;
        for( int i=0 ; i<WNAX ; i++ ){
            win[i][WNAY] = new Window( OMEGA, xpin, ypin );
            xpin=xpin+OMEGA;
        }
    }

    if( width%OMEGA!=0 ){
        xpin=X2-OMEGA;
        ypin=Y1;
        for( int j=0 ; j<WNAY ; j++ ){
            win[WNAX][j] = new Window( OMEGA, xpin, ypin );
            ypin=ypin+OMEGA;
        }
    }

    if( width%OMEGA!=0 && height%OMEGA!=0 ){
        xpin=X2-OMEGA;
        ypin=Y2-OMEGA;
        win[WNAX][WNAY] = new Window( OMEGA, xpin, ypin );
    }
    windows = win;
}

Map::~Map()
{
    for(size_t i=0; i<groups.size(); ++i) delete groups[i];
}

void Map::makeGroup()
{
    int pos[4];
    size_t n = graph->getNumofNode();
    graph->nodes[0]->setGref();
    graph->nodes[0]->setGref2();
    for(size_t i=0;i<n;++i){
        Node* h = graph->nodes[i];
        if(!h->isGref()){
            if(graph->coloring(h,pos)){
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
        if( (groups[h]->x2)%OMEGA==0 )
            o=(groups[h]->x2)/OMEGA-1;
        else    
            o=(groups[h]->x2)/OMEGA;
        if( (groups[h]->y2)%OMEGA==0 )
            p=(groups[h]->y2)/OMEGA-1;
        else
            p=(groups[h]->y2)/OMEGA;
        for( int i=m ; i<=o ; i++ ){
            for( int j=n ; j<=p ; j++ ){
                if( CheckSharing( groups[h], windows[i][j] ) )
                    windows[i][j]->wgroups.push_back(groups[h]);
            }
        }
    }
}

bool Map::CheckSharing( Group* g, Window* w ){
    bool check=false;
    size_t length=g->nodes.size();
    for( size_t i=0 ; i<length ; i++ ){
        if( g->nodes[i]->x1 > (w->WX)+(OMEGA) ) continue;
        if( g->nodes[i]->y1 > (w->WY)+(OMEGA) ) continue;
        if( g->nodes[i]->x2 < (w->WX) ) continue;
        if( g->nodes[i]->y2 < (w->WY) ) continue;
        check=true;
        break;
    }
    return check;
}

int Map::numberofWindow(Donegroup& dg, Group* g){
    int pos[4];
    getWindowNumber(dg, g, pos);
    groups[0]->setGref2();
    int sum=0;
	
    for(int i=pos[0];i<=pos[2];++i){
        for(int j=pos[1];j<=pos[3];++j){
            Window* w=windows[i][j];
            if(!w->isGref()){
                size_t nmax=w->wgroups.size();
                for(size_t n=0;n<nmax;++n){
	                if(!w->wgroups[n]->isGref()){
                        if(!w->wgroups[n]->isGref2()){
                            ++sum;
                            w->wgroups[n]->setToGref2();
                        }
                    }
	            }
	        }
        }
    }
	
    return sum;
}

void Map::tryBest(Donegroup& dg, Group* g){
    int pos[4];
    getWindowNumber(dg,g,pos);
    groups[0]->setGref2();
    vector<Group*> gptr;
	
    for(int i=pos[0];i<=pos[2];++i){
        for(int j=pos[1];j<=pos[3];++j){
            Window* w=windows[i][j];
            if(!w->isGref()){
                size_t nmax=w->wgroups.size();
                for(size_t n=0;n<nmax;++n){
                    if(!w->wgroups[n]->isGref()){
                        if(!w->wgroups[n]->isGref2()){
                            gptr.push_back(w->wgroups[n]);
                            w->wgroups[n]->setToGref2();
                        }
                    }
                }
            }
        }
    }

//try all possibility
    size_t max=gptr.size();
    size_t mask[max-1];
    mask[0]=1;
    for(size_t n=1;n<max;++n){
        mask[n]=mask[n-1]<<1;
    }
	
    size_t best;
    int mincolordiff=numeric_limits<int>::max();
    
    windows[0][0]->setGref();
    
    for(size_t m=0;m<mask[max-1];++m){
        groups[0]->setGref2();
        for(size_t n=1;n<max;++n) if((m&mask[n-1])>>n) gptr[n]->setToGref2();

        int colordiff=0;	
        for(int i=pos[0];i<=pos[2];++i){
            for(int j=pos[1];j<=pos[3];++j){
                Window* w=windows[i][j];
                if(!w->isGref()){
                    size_t nmax=w->wgroups.size();
                    int wsum=0;
                    for(size_t n=0;n<nmax;++n){
                        if(w->wgroups[n]->isGref()){
                            if(w->wgroups[n]->rev)  wsum-=w->color[n];
                            else  wsum+=w->color[n];
                        }else{
                            if(w->wgroups[n]->isGref2()) wsum+=w->color[n];
                            else wsum-=w->color[n];
                        }
                    }
                    if(wsum<0)wsum=-wsum;
                    colordiff+=wsum;
                }
            }
        }
		
        if(colordiff<mincolordiff){
            best=m;
            mincolordiff=colordiff;
        }
    }

//set to the best choice and mark ref
    groups[0]->setGref2();
    for(size_t n=0;n<max;++n){
        if((best&mask[n-1])>>n) gptr[n]->setToGref2();
    }	

    for(size_t n=0;n<max;++n){
        if(!gptr[n]->isGref2()){
            gptr[n]->reverse();
            gptr[n]->rev=true;   
        }
        gptr[n]->setToGref();
    }
    
    for(int i=pos[0];i<=pos[1];++i)
        for(int j=pos[2];j<=pos[3];++j) 
            windows[i][j]->setToGref();
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
    
    groups[0]->setToGref();
    Donegroup dg(groups[0]);

    for(size_t i=1;i<n;){
        size_t j=i;
	    size_t jtemp=i;
	    int maxnumtemp = numeric_limits<int>::max();
	    bool notexist=true;
    
	    for(;j<n&&j<i+n/range+1;++j){			
	        if(!groups[j]->isGref()){
                notexist=false;
                int num=numberofWindow(dg, groups[j]);
                if(num<maxnum) break;
                else if(num<maxnumtemp){
                    jtemp=j;
                    maxnumtemp=num;
                }
                if(j==n-1||j==i+n/range){
                    j=jtemp;
                    maxnum=maxnumtemp;
                    break;
                }
            }
        }
        if(notexist) i=(n>i+n/range+1)?i+n/range+1 :n;
        else{
            if(i==j) ++i;
            tryBest(dg, groups[j]);
            dg.update(groups[j]);
        }
    }
}

void Map::getWindowNumber(Donegroup& dg, Group* g, int* result){
    int xpin1;
    int ypin1;
    int xpin2;
    int ypin2;
    if( dg.x1 < g->x1 ) xpin1=dg.x1;
    else xpin1=g->x1;
    if( dg.x2 > g->x2 ) xpin2=dg.x2;
    else xpin2=g->x2;
    if( dg.y1 < g->y1 ) ypin1=dg.y1;
    else ypin1=g->y1;
    if( dg.y2 > g->y2 ) ypin2=dg.y2;
    else ypin2=g->y2;
    result[0]=(xpin1)/OMEGA;
    result[1]=(ypin1)/OMEGA;
    if( xpin2%OMEGA==0 )
        result[2]=xpin2/OMEGA-1;
    else    
        result[2]=xpin2/OMEGA;
    if( ypin2%OMEGA==0 )
        result[3]=ypin2/OMEGA-1;
    else
        result[3]=ypin2/OMEGA;
}

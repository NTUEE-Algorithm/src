#include "map.h"
#include "graph.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;
int Group::gref = 0;
int Group::gref2 = 0;

Group::Group(Node* h)
{
    head = h;
    effect = 0;
    ref = gref;
    ref2 = gref2;
    rev = false;
}

void Group::setPos(int* pos)
{
    x1 = pos[0];
    y1 = pos[1];
    x2 = pos[2];
    y2 = pos[3];
}

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
    ++gref;
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
    ++gref2;
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
    size_t length_g=wgroups.size();
    for( int i=0 ; i<length_g ; ++i ){
        int sum1=0;
        int sum2=0;
        size_t length_n=wgroups[i]->nodes.size();
        for( int j=0 ; j<length_n ; ++j ){
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
              else
                  effx1=wgroups[i]->nodes[j]->x1;
              if( wgroups[i]->nodes[j]->x2 > WX+OMEGA )
                  effx2=WX+OMEGA;
              else
                  effx2=wgroups[i]->nodes[j]->x2;
              if( wgroups[i]->nodes[j]->y1 < WY )
                  effy1=WY;
              else
                  effy1=wgroups[i]->nodes[j]->y1;
              if( wgroups[i]->nodes[j]->y2 > WY+OMEGA )
                  effy2=WY+OMEGA;
              else
                  effy2=wgroups[i]->nodes[j]->y2;
                
              area=(effx2-effx1)*(effy2-effy1);
              if( wgroups[i]->nodes[j]->color )
                  sum1=sum1+area;
              else
                  sum2=sum2+area;
        }
        color.push_back(sum1-sum2);
        color1.push_back(sum1);
        color2.push_back(sum2); 
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
    ++gref;
}

void Map::CreatWindow(){
    OMEGA = graph->omega;
    int WNAX_2   = X2/OMEGA;   //  window numbers along x without the last one
    int WNAY_2   = Y2/OMEGA;   //  window numbers along y without the last one
    int xpin   = 0;
    int ypin   = 0;
    Window*** win;
    
    if( X2%OMEGA!=0 && Y2%OMEGA!=0 ){
        win = new Window** [WNAX_2+1];
        for( int i=0 ; i<WNAX_2+1 ; ++i )
            win[i] = new Window* [WNAY_2+1];
    }
    else if( X2%OMEGA==0 && Y2%OMEGA!=0 ){
        win = new Window** [WNAX_2];
        for( int i=0 ; i<WNAX_2 ; ++i )
            win[i] = new Window* [WNAY_2+1];
    }
    else if( X2%OMEGA!=0 && Y2%OMEGA==0 ){
        win = new Window** [WNAX_2+1];
        for( int i=0 ; i<WNAX_2+1 ; ++i )
            win[i] = new Window* [WNAY_2];
    }
    else if( X2%OMEGA==0 && Y2%OMEGA==0 ){
        win = new Window** [WNAX_2];
        for( int i=0 ; i<WNAX_2 ; ++i )
            win[i] = new Window* [WNAY_2];
    }
    

    for( int i=0 ; i<WNAX_2 ; ++i ){
        ypin=0;
        for( int j=0 ; j<WNAY_2 ; ++j ){
            win[i][j] = new Window ( OMEGA, xpin, ypin );
            ypin=ypin+OMEGA;
        }
        xpin=xpin+OMEGA;
    }

    if( Y2%OMEGA!=0 ){
        xpin=0;
        ypin=Y2-OMEGA;
        for( int i=0 ; i<WNAX_2 ; ++i ){
            win[i][WNAY_2] = new Window( OMEGA, xpin, ypin );
            xpin=xpin+OMEGA;
        }
    }

    if( X2%OMEGA!=0 ){
        xpin=X2-OMEGA;
        ypin=0;
        for( int j=0 ; j<WNAY_2 ; ++j ){
            win[WNAX_2][j] = new Window( OMEGA, xpin, ypin );
            ypin=ypin+OMEGA;
        }
    }

    if( X2%OMEGA!=0 && Y2%OMEGA!=0 ){
        xpin=X2-OMEGA;
        ypin=Y2-OMEGA;
        win[WNAX_2][WNAY_2] = new Window( OMEGA, xpin, ypin );
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
            bool valid=true;
            int pos[4];
            Group* temp=new Group(h);
            graph->coloring(h,pos,temp->nodes,valid);
            temp->setPos(pos);
            
            if(valid) groups.push_back(temp);
            else ngroups.push_back(temp);
        }
    }
}

void Map::InitXY(){
    size_t Glength=groups.size();
    int xpin1=numeric_limits<int>::max();
    int ypin1=numeric_limits<int>::max();
    int xpin2=numeric_limits<int>::min();
    int ypin2=numeric_limits<int>::min();
    for( size_t i=0 ; i<Glength ; ++i ){
        if((groups[i]->x1)<xpin1) xpin1=(groups[i]->x1);
        if((groups[i]->y1)<ypin1) ypin1=(groups[i]->y1);
        if((groups[i]->x2)>xpin2) xpin2=(groups[i]->x2);
        if((groups[i]->y2)>ypin2) ypin2=(groups[i]->y2);
    }
    X1 = xpin1;
    Y1 = ypin1;
    for( size_t i=0 ; i<Glength ; ++i ){        
        (groups[i]->x1)=(groups[i]->x1)-xpin1;
        (groups[i]->y1)=(groups[i]->y1)-ypin1;
        (groups[i]->x2)=(groups[i]->x2)-xpin1;
        (groups[i]->y2)=(groups[i]->y2)-ypin1;
        size_t Nlength=groups[i]->nodes.size();
        for( size_t j=0 ; j<Nlength ; ++j ){
            (groups[i]->nodes[j]->x1)=(groups[i]->nodes[j]->x1)-xpin1;
            (groups[i]->nodes[j]->y1)=(groups[i]->nodes[j]->y1)-ypin1;
            (groups[i]->nodes[j]->x2)=(groups[i]->nodes[j]->x2)-xpin1;
            (groups[i]->nodes[j]->y2)=(groups[i]->nodes[j]->y2)-ypin1;
        }
    }
    X2=xpin2-xpin1;
    Y2=ypin2-ypin1;
    WNAX = (X2)/OMEGA;   //  window numbers along x without the last one
    WNAY = (Y2)/OMEGA;   //  window numbers along y without the last one
    if( (X2)%OMEGA!=0 ) ++WNAX;
    if( (Y2)%OMEGA!=0 ) ++WNAY;
}
void Map::BuildAllColor(){
    OMEGA = graph->omega;       //  can be delete if OMEGA is already set
    for( int i=0 ; i<WNAX ; ++i ){
        for( int j=0 ; j<WNAY ; ++j ){
            windows[i][j]->BuildColor();        
        }
    }
}

void Map::InitEffect(){
    OMEGA = graph->omega;       //  can be delete if OMEGA is already set
    for( int i=0 ; i<WNAX ; ++i ){
        for( int j=0 ; j<WNAY ; ++j ){
            size_t ColorLength=windows[i][j]->color.size();
            Window* w=windows[i][j];
            for( int k=0 ; k<ColorLength ; ++k )
                (w->wgroups[k]->effect)=(w->wgroups[k]->effect)+MinMax(w->color, k);           
        }
    }
}

int Map::MinMax( vector<int>& v, int& skip ){
    size_t pow = v.size();
    size_t counter=1;
    size_t temp=0;
    int MinCompare=numeric_limits<int>::max();
    int MaxCompare=0;
    int sum=0;
    
    if( pow==1 )
        return 0;

    size_t* mask=new size_t[pow];
    mask[0]=1;
    for( size_t i=1 ; i<pow-1 ; ++i )
        mask[i]=mask[i-1]<<1;

    for( size_t i=0 ; i<pow-2 ; ++i )
        counter=counter<<1;
    
    for( size_t i=0 ; i<counter ; ++i ){
        sum=0;
        temp=0;
        for( size_t j=0 ; j<pow ; ++j ){
            if(j!=skip){
                if(i&mask[temp]) sum=sum+v[j];      
                else             sum=sum-v[j];
                ++temp; 
            }
        }
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
    
    for( size_t h=0 ; h<Glength ; ++h ){
        m=(groups[h]->x1)/OMEGA;
        n=(groups[h]->y1)/OMEGA;
        //    check where the groups is    //
        if( (groups[h]->x2)%OMEGA==0 )
            o=(groups[h]->x2)/OMEGA-1;
        else{
            if( (groups[h]->x2)>X2-OMEGA )
                o=WNAX-1; 
            else
                o=(groups[h]->x2)/OMEGA;
        } 
        if( (groups[h]->y2)%OMEGA==0 )
            p=(groups[h]->y2)/OMEGA-1;
        else{
            if( (groups[h]->y2)>Y2-OMEGA )
                p=WNAY-1; 
            else
                p=(groups[h]->y2)/OMEGA;
        }           
        //    Start linking    //
        for( size_t i=m ; i<=o ; ++i ){
            for( size_t j=n ; j<=p ; ++j ){
                if( CheckSharing( groups[h], windows[i][j] ) )
                    windows[i][j]->wgroups.push_back(groups[h]);
            }
        }               
    }
}

bool Map::CheckSharing( Group* g, Window* w ){
    bool check=false;
    size_t length=g->nodes.size();
    for( size_t i=0 ; i<length ; ++i ){
        if( g->nodes[i]->x1 > (w->WX)+(OMEGA) ) continue;
        if( g->nodes[i]->y1 > (w->WY)+(OMEGA) ) continue;
        if( g->nodes[i]->x2 < (w->WX) ) continue;
        if( g->nodes[i]->y2 < (w->WY) ) continue;
        check=true;
        break;
    }
    return check;
}

int Map::numberofGroup(Donegroup& dg, Group* g){
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
    size_t mask[max+1];
    mask[0]=1;
    for(size_t n=1;n<max+1;++n){
        mask[n]=mask[n-1]<<1;
    }
    size_t best;
    int mincolordiff=numeric_limits<int>::max();
 
    for(size_t m=0;m<mask[max];++m){       
        groups[0]->setGref2();
        for(size_t n=0;n<max;++n) if(m&mask[n]) gptr[n]->setToGref2();

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
        if(best&mask[n]) gptr[n]->setToGref2();
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
    windows[0][0]->setGref();
	
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
                int num=numberofGroup(dg, groups[j]);
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
            tryBest(dg, groups[j]);
            dg.update(groups[j]);
            if(groups[i]->isGref()) ++i;
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
    if( (xpin2)%OMEGA==0 )
        result[2]=(xpin2)/OMEGA-1;
    else{
        if( (xpin2)>X2-OMEGA )
            result[2]=WNAX-1; 
        else
            result[2]=(xpin2)/OMEGA;
    }  
    if( (ypin2)%OMEGA==0 )
        p=(ypin2)/OMEGA-1;
    else{
        if( (ypin2)>Y2-OMEGA )
            result[3]=WNAY-1; 
        else
            result[3]=(ypin2)/OMEGA;
    } 
}

double Map::printFile(fstream& output)
{
    int xNum, yNum;
    if (!(X2%OMEGA))
        xNum = X2/OMEGA;
    else
        xNum = X2/OMEGA + 1;
    if (!(Y2%OMEGA))
        yNum = Y2/OMEGA;
    else
        yNum = Y2/OMEGA + 1;    
    int count = 1;
    double score = 100;
    for (int i=0; i<yNum; ++i) 
        for (int j=0; j<xNum; ++j) {
            output << "WIN[" << count << "]=";
            score -= windows[j][i]->print(output, this);
            ++count;
        }
    for (size_t i=0; i<ngroups.size(); ++i) {
        output << "GROUP" << endl;    
        for (size_t j=0; j<ngroups[i]->nodes.size(); ++j)
        {
           output << "NO[" << j+1 << "]=" << ngroups[i]->nodes[j]->x1 << "," 
                  << ngroups[i]->nodes[j]->y1 << "," << ngroups[i]->nodes[j]->x2 << ","
                  << ngroups[i]->nodes[j]->y2 << endl;
        }
    }
    for (size_t i=0; i<groups.size(); ++i) {
        output << "GROUP" << endl;
        vector<Node*> CA;
        vector<Node*> CB;
        for (size_t j=0; j<groups[i]->nodes.size(); ++j)
            if (groups[i]->nodes[j]->color==0)
                CA.push_back(groups[i]->nodes[j]);
            else
                CB.push_back(groups[i]->nodes[j]);
        for (size_t j=0; j<CA.size(); ++j)
            output << "CA[" << j+1 << "]=" 
                   << CA[j]->x1+X1 << "," << CA[j]->y1+Y1 << "," 
                   << CA[j]->x2+X1 << "," << CA[j]->y2+Y1 << endl;
        for (size_t j=0; j<CB.size(); ++j)
            output << "CB[" << j+1 << "]=" 
                   << CB[j]->x1+X1 << "," << CB[j]->y1+Y1 << "," 
                   << CB[j]->x2+X1 << "," << CB[j]->y2+Y1 << endl;                   
    }
    return score;
}

double Window::print(fstream& output, Map* map)
{
    double sum1 = 0, sum2 = 0;
    for (size_t i=0; i<color.size(); ++i) {
       if (wgroups[i]->rev) {
           sum1 += color2[i];
           sum2 += color1[i];    
       }
       else {
           sum1 += color1[i];
           sum2 += color2[i];    
       }
    }
    sum1 = (sum1/OMEGA/OMEGA)*100;
    sum2 = (sum2/OMEGA/OMEGA)*100;
    output << WX+map->X1 << "," << WY+map->Y1 << "," << WX+OMEGA+map->X1 
           << "," << WY+OMEGA+map->Y1 << "(" << fixed << setprecision(2) << sum1 
           << " " << fixed << setprecision(2) << sum2 << ")" << endl;
    return abs(sum1-sum2);
}

void Map::optSolver(){
    size_t max=groups.size()-1;
    size_t mask[max+1];
    mask[0]=1;
    for(size_t n=1;n<max+1;++n){
        mask[n]=mask[n-1]<<1;
    }
    size_t best;
    int mincolordiff=numeric_limits<int>::max();
 
    for(size_t m=0;m<mask[max];++m){       
        groups[0]->setGref2();
        for(size_t n=0;n<max;++n) if(m&mask[n]) groups[n]->setToGref2();

        int colordiff=0;	
        for(int i=0;i<WNAX;++i){
            for(int j=0;j<WNAY;++j){
                Window* w=windows[i][j];
                size_t nmax=w->wgroups.size();
                int wsum=0;
                for(size_t n=0;n<nmax;++n){
                    if(w->wgroups[n]->isGref2()) wsum+=w->color[n];
                    else wsum-=w->color[n];    
                }
                if(wsum<0)wsum=-wsum;
                colordiff+=wsum;
            }
        }
        if(colordiff<mincolordiff){
            best=m;
            mincolordiff=colordiff;
        }
    }

//set to the best choice
    groups[0]->setGref2();
    for(size_t n=0;n<max;++n){
        if(best&mask[n]) groups[n]->setToGref2();
    }	
    for(size_t n=0;n<max+1;++n){
        if(!groups[n]->isGref2()){
            groups[n]->reverse();
            groups[n]->rev=true;   
        }
    }
}

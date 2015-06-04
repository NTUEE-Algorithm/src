#include "graph.h"
#include <iostream>

Edge::Edge(Node *a, Node *b)
{
	if ( a->id <= b->id ) { node[0] = a; node[1] = b; }
	else { node[0] = b; node[1] = a; }
}

bool Edge::operator < (const Edge& rhs) const{

	int id1a = node[0]->id;
	int id1b = node[1]->id;

	int id2a = rhs.node[0]->id;
	int id2b = rhs.node[1]->id;


	if ( id1a == id2a ) return id1b < id2b;
	if ( id1a == id2b ) return id1b < id2a;
	if ( id1b == id2a ) return id1a < id2b;
	if ( id1b == id2b ) return id1a < id2a;
    return true;
}
Node * Edge::getNeighbor(Node *n)
{
	if ( node[0] == n ) return node[1];
	if ( node[1] == n ) return node[0];

	return 0;	
}

Node::gref = 0;
Node::gref2 = 0;

Node::Node(const int i, int* c)
{
   id = i;
   color = 0;
   x1 = c[0];
   y1 = c[1];
   x2 = c[2];
   y2 = c[3];
   ref = gref;
   ref2 = gref2;
   group = 0;
}

void Node::addEdge(Edge *e)
{
	edge.push_back(e);
}

bool edgeComp( const Edge* A, const Edge* B ){
	if ( (*A) < (*B) ) return true;
	return false;
}


void Node::sortEdge()
{
    sort(edge.begin(), edge.end(), edgeComp);
}

bool Node::isGref()
{
    return (ref == gref);
}

void Node::setToGref()
{
    ref = gref;
}

void Node::setGref()
{
    gref++;
}

bool Node::isGref2()
{
    return (ref2 == gref2);
}

void Node::setToGref2()
{
    ref2 = gref2;
}

void Node::setGref2()
{
    gref2++;
}

Graph::~Graph()
{
	vector<Edge *>::iterator itE;
	for ( itE = edges.begin() ; itE != edges.end() ; itE++ )
	{
		delete (*itE);
		(*itE) = 0;
	}
	
	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
	{
		delete (*itN).second;
		(*itN).second = 0;
	}

    vector<Node *>::iterator itN2;
	for ( itN2 = nodes.begin() ; itN2 != nodes.end() ; itN2++ )
	{
		(*itN2) = 0;
	}
}

void Graph::addEdge(Node* v1, Node* v2)
{
	Edge *e = new Edge(v1, v2);
	edges.push_back(e);

	v1->edge.push_back(e);
	v2->edge.push_back(e);
}

void Graph::sortEdgesOfNode()
{
	map<int, Node *>::iterator it;
	for ( it = nodesMap.begin() ; it != nodesMap.end() ; it++ )
	{
		Node *node = (*it).second;
		node->sortEdge();
	}

}

bool NodeCompByID( const Node* A, const Node* B ){
	if ( A->id < B->id ) return true;
    else return false;
}

void Graph::sortNodesByID()
{
    sort(nodes.begin(), nodes.end(), NodeCompByID);
}

bool NodeCompByX1( const Node* A, const Node* B ){
	if ( A->x1 < B->x1 ) return true;
    else return false;
}

void Graph::sortNodesByX1()
{
    sort(nodes.begin(), nodes.end(), NodeCompByX1);
}

bool NodeCompByY1( const Node* A, const Node* B ){
	if ( A->y1 < B->y1 ) return true;
    else return false;
}

void Graph::sortNodesByY1()
{
    sort(nodes.begin(), nodes.end(), NodeCompByY1);
}

void Graph::addEdgesInX()
{
   sortNodesByY1();
   for (size_t i=0; i<nodes.size(); ++i)
      for (size_t j=i+1; j<nodes.size() && nodes[i]->y2>nodes[j]->y1; ++j)
         if (nodes[i]->x1 > nodes[j]->x1) {
            if (nodes[i]->x1-nodes[j]->x2 < alpha)
               addEdge(nodes[i],nodes[j]);
         }
         else {
            if (nodes[j]->x1-nodes[i]->x2 < alpha)
               addEdge(nodes[i],nodes[j]);
         }
}

void Graph::addEdgesInY()
{
   sortNodesByX1();
   for (size_t i=0; i<nodes.size(); ++i)
      for (size_t j=i+1; j<nodes.size() && nodes[i]->x2>nodes[j]->x1; ++j)
         if (nodes[i]->y1 > nodes[j]->y1) {
            if (nodes[i]->y1-nodes[j]->y2 < beta)
               addEdge(nodes[i],nodes[j]);
         }
         else {
            if (nodes[j]->y1-nodes[i]->y2 < beta)
               addEdge(nodes[i],nodes[j]);
         }
}

void Graph::init()
{
	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
	{
		Node *node = (*itN).second;
                node->color = -1;
	}
}

Node * Graph::getNodeById(const int& id)
{
	return nodesMap[id];
}

size_t Graph::getNumofNode()
{
    return nodes.size();	
}

bool Graph::coloring(Node* h, int* pos)
{
    if(!h->isGref())
    {
        h->setToGref();
        h->color = 0;
        pos[0] = h->x1;
        pos[1] = h->y1;
        pos[2] = h->x2;
        pos[3] = h->y2;
    }
    
    size_t n = h->edge.size();

    for(size_t i = 0;i<n;++i)
    {
        Node* temp = h->edge[i]->getNeighbor(h);
        if(!temp->isGref())
        {
            temp->color = ((h->color)?0:1);
            if(temp->x1<pos[0]) pos[0] = temp->x1;
            if(temp->y1<pos[1]) pos[1] = temp->y1;
            if(temp->x2>pos[2]) pos[2] = temp->x2;
            if(temp->y2>pos[3]) pos[3] = temp->y2;
            temp->setToGref();
            if(!coloring(temp,pos)) return false;
        }else
        {
            if(temp->color==h->color) return false;
        }
    }
    return true;
}

void Graph::markAll(Node* h)
{
    h->setToGref();
    h->setToGref2();
   
    size_t n = h->edge.size();
    for(size_t i = 0;i<n;++i)
    {
         Node* temp = h->edge[i]->getNeighbor(h);
         if(!temp->isGref2()) markAll(temp);
    }	
}

void Graph::setGroup(Node* h)
{
	
}

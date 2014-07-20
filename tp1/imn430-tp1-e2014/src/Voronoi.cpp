
//  Voronoi.cpp
//  IMN430-TP1
//
//  Created by François Bélanger on 2014-05-13.
//  Copyright (c) 2014 François Bélanger 94 245 437. All rights reserved.
//

//TODO: renommer les fichiers !!!

#include <set>
#include <queue>
#include <limits>
#include <cmath>

//FIXME to remove
#include <iostream>

#include "Voronoi.h"
#include "DCEL.h"


//---------------SiteEvent---------------
VoronoiDiagram::SiteEvent::SiteEvent(DCEL::Vertex* pt)
    : VoronoiDiagram::VoronoiEvent(pt){
}

bool VoronoiDiagram::SiteEvent::isSiteEvent()const{
    return true;
}

//---------------CircleEvent---------------
VoronoiDiagram::CircleEvent::CircleEvent(DCEL::Vertex* triggerPoint, DCEL::Vertex center, const VoronoiDiagram::tree_type* tNode)
    : VoronoiDiagram::VoronoiEvent(triggerPoint)
    , node(tNode)
    , mCenter(center)
{
}
bool VoronoiDiagram::CircleEvent::isSiteEvent()const{
    return false;
}

//---------------VoronoiDiagram---------------
void VoronoiDiagram::addSite(DCEL::Vertex vertex)
{
	//Define bounding box
	bbMin.x = bbMin.x < vertex.x ? bbMin.x : vertex.x;
    bbMin.y = bbMin.y < vertex.y ? bbMin.y : vertex.y;
    bbMax.x = bbMax.x > vertex.x ? bbMax.x : vertex.x;
    bbMax.y = bbMax.y > vertex.y ? bbMax.y : vertex.y;
    
    sites.insert(new DCEL::Vertex(vertex));
}

void VoronoiDiagram::clearDiagram()
{
    while(!mEventQueue.empty())
    {
        mEventQueue.pop();
    }
    
    mStatusTree.clear();
    edges.clear();
    siteToRegion.clear();
    vertices.clear();
}

void VoronoiDiagram::fortuneAlgorithm(){
    clearDiagram();
    
    if(sites.empty()){
        return;
    }
    
    //Step1 : Initialize the event queue ith all sites events
    for(auto iter = sites.begin(); iter != sites.end(); iter++){
		VoronoiEvent* anEvent = new SiteEvent(*iter);
        mEventQueue.push(anEvent);
    }
    
    //Step2 : while the event queue is not empty
    VoronoiEvent* event = nullptr;
    while(!mEventQueue.empty()){
		//Step 3 : Remove the event with largest y-coordinate from the event queue
        event = mEventQueue.top();
        mEventQueue.pop();
        
        if(event->isValid()){
            //Update fortune's line
			line = event->getPoint()->y;
        
			//Step 4 : If the event is a Site event, Step 5 : call handleSiteEvent
            if(event->isSiteEvent())
				handleSiteEvent(event->getPoint());
   
			//Step 6 : Else, call handleCircleEvent(leaf with arc that will disapear)
            else
				handleCircleEvent(reinterpret_cast<CircleEvent*>(event));
            
        }

		//Step 7 : Compute a bounding box that contains all vertices of the Voronoi diagram in its interior.
		//		   Attach the half - inﬁnite edges to the bounding box by updating the doubly - connected edge list appropriately.
		finishEdge(); ////////////////////////////////////////////////////////////////////////

		//Step 8 : Traverse the half-edges of the doubly-connected edge list to add the cell records and the pointers to and from them.
        delete event;
    }
    //return edges;
}

/*
	Note : If there's s1 and s2 are colinear in x, there will only be one intersection. 
		   This means i1 and i2 are going to be equal.
*/
void VoronoiDiagram::getIntersection(DCEL::Vertex* s1, DCEL::Vertex* s2, DCEL::Vertex* i1, DCEL::Vertex* i2, const double y)const
{
    //Get parabola distance
    double dp1 = 2.0 * (s1->y - y);
    const double a1 = 1.0 / dp1;
    const double b1 = -2.0 * s1->x / dp1;
    const double c1 = y + dp1 / 4 + s1->x * s1->x / dp1;
 
    //Get parabola distance
    double dp2 = 2.0 * (s2->y - y);
    const double a2 = 1.0 / dp2;
    const double b2 = -2.0 * s2->x/dp2;
    const double c2 = y + dp2 / 4 + s2->x * s2->x / dp2;
    
    //Compare Equation
    const double a = a1 - a2;
    const double b = b1 - b2;
    const double c = c1 - c2;
 
    //Compute discriminant
    const double disc = b*b - 4 * a * c;
    const double _2_A = 2*a;
    const double sqrt_disct = std::sqrt(disc);
    
    //Get point value
    i1->x = (-b + sqrt_disct) / _2_A;
    i2->x = (-b - sqrt_disct) / _2_A;
    i1->y = a1*(i1->x*i1->x) + b1*i1->x + c1;
    i2->y = a2*(i2->x*i2->x) + b2*i2->x + c2;
    
}

void VoronoiDiagram::handleSiteEvent(DCEL::Vertex* site){
    
	//Step 1 : If the tree is empty, insert the site into it and return
	if(mStatusTree.empty()){
        mStatusTree.insert(tree_type(site, nullptr));
        return;
    }
    
    double minYInter = std::numeric_limits<double>::max();
    double yIntersec = minYInter;
    
	//Step 2.A : Compute the intersection with the parabola directly above the site
    status_type::iterator intersectedParabola = mStatusTree.begin();
    
    for(status_type::iterator i = mStatusTree.begin(); i != mStatusTree.end(); ++i)
    {
        if(!(i->X() == site->x && i->Y() == site->y))
        {
            const double xDiff = i->X() - site->x;
            const double yDiff = i->Y() - site->y;
            yIntersec = ((xDiff*xDiff)+ (i->Y()*i->Y()) - (site->y*site->y));
            yIntersec /= (2*yDiff);
            if(minYInter > yIntersec)
            {
                minYInter = yIntersec;
                intersectedParabola = status_type::iterator(i);
            }
        }
    }
	//Step 2.B : If the leaf representing the parabola has a pointer to a circle event in Q, then this circle event is a false alarm
    if(intersectedParabola->event != nullptr)
        intersectedParabola->event->setValid(false);
    
    //Step 3 : insert the new site in the tree 
    auto insertPos = (mStatusTree.insert(tree_type(site,nullptr)).first);
    
    //Step 4 : Create new half-edge records in the Voronoi diagram structure

	//Create a region
    createEdges(site, intersectedParabola->site, DCEL::Vertex(site->x, minYInter));
    
	//-->Create new half-edge records in the Voronoi diagram structure for the edge separating V(pi) and V(pj), which will be traced out by the two new breakpoints.

    
	//Step 5 : Compute circles
    //the left one
    if(intersectedParabola != mStatusTree.begin())
        checkCircleEvent(prev(intersectedParabola), &(*insertPos),site->y);
    
    //the middle one
    checkCircleEvent(intersectedParabola, &(*insertPos), site->y);
    
    //the right one
    if(next(intersectedParabola) != mStatusTree.end())
        checkCircleEvent(next(intersectedParabola), &(*insertPos), site->y);
}

//code very lightly modifed from http://www.cs.hmc.edu/~mbrubeck/voronoi.html
bool VoronoiDiagram::computeCircle(DCEL::Vertex p1, DCEL::Vertex p2, DCEL::Vertex p3, double *radius, DCEL::Vertex *center)
{
    //FIXME: check
    // Check that bc is a "right turn" from ab.
    //if ((p2.x-p1.x)*(p3.y-p1.y) - (p3.x-p1.x)*(p2.y-p1.y) > 0)
    //    return false;
    
    // Algorithm from O'Rourke 2ed p. 189.
    double A = p2.x - p1.x,  B = p2.y - p1.y,
    C = p3.x - p1.x,  D = p3.y - p1.y,
    E = A*(p1.x+p2.x) + B*(p1.y+p2.y),
    F = C*(p1.x+p3.x) + D*(p1.y+p3.y),
    G = 2*(A*(p3.y-p2.y) - B*(p3.x-p2.x));
    
    if (G == 0) return false;  // Points are co-linear.
    
    // Compute the center of the circle.
    center->x = (D*E-B*F)/G;
    center->y = (A*F-C*E)/G;
    
    // o.x plus radius equals max x coordinate.
    *radius = center->y + sqrt( pow(p1.x - center->x, 2) + pow(p1.y - center->y, 2) );
    return true;
}

void VoronoiDiagram::checkCircleEvent(status_type::iterator parabola, const tree_type* node, double y)
{
    //If there's no site before or after the parabola, do not compute...
    if ((parabola == mStatusTree.begin()) || (parabola == std::prev(mStatusTree.end())))
        return;
    
    status_type::iterator tmp = prev(parabola);
    DCEL::Vertex a(tmp->X(), tmp->Y());
    DCEL::Vertex b(parabola->X(), parabola->Y());
    tmp = next(parabola);
    DCEL::Vertex c(tmp->X(), tmp->Y());
    
    DCEL::Vertex circleCenter;
    double circleRadius;
    
    if (computeCircle(a, b, c, &circleRadius, &circleCenter))// && (circleCenter.y - circleRadius < y))
    {
        //Create a pointer to a new circle event
        CircleEvent* newEvent = new CircleEvent(new DCEL::Vertex(circleCenter.x, circleCenter.y - circleRadius), circleCenter, &(*parabola));
        const_cast<tree_type*>(&(*parabola))->event = newEvent;
        
        //Add the event in the event queue
        mEventQueue.push(parabola->event);
    }
}

void VoronoiDiagram::handleCircleEvent(VoronoiDiagram::CircleEvent* event)
{
    if(!event->isValid()) //sanity check
    {
        return;
    }
    
    DCEL::Vertex* center = new DCEL::Vertex(event->mCenter);
    
    //Step1
    //Fuck alex!!! et ses ITS
    auto iter = mStatusTree.find(*(event->node));
    auto leftSite = mStatusTree.end();
    auto rightSite = mStatusTree.end();
    
    if(iter != mStatusTree.begin())
    {
        leftSite = prev(iter);
        
        if(leftSite->event != nullptr)
        {
            leftSite->event->setValid(false);
            const_cast<tree_type*>(&(*leftSite))->event = nullptr;
        }
    }
    
    if(iter != mStatusTree.end())
    {
        rightSite = next(iter);
        
        if(rightSite->event != nullptr)
        {
            rightSite->event->setValid(false);
            const_cast<tree_type*>(&(*rightSite))->event = nullptr;
        }
    }
    
    //Step 2
    vertices.push_back(event->mCenter);
    
    //Attatch the 3 new record to the half-edge that end at the vertex
    DCEL::Edge* prevSiteEdge = getEdge(getRegion(*(event->node->getPoint())), getRegion(*(leftSite->getPoint())));
    DCEL::Edge* nextSiteEdge = getEdge(getRegion(*(event->node->getPoint())), getRegion(*(rightSite->getPoint())));
    
    //create 2 half-edge record corresponding to the new breakpoint on the beach line.
    //set the pointer between them
    DCEL::Edge* newEdge = createEdges(leftSite->getPoint(), rightSite->getPoint(), *center);
    
    //Handle disapearing site.
    DCEL::Vertex* v = new DCEL::Vertex(event->mCenter);
    nextSiteEdge->setOrigin(v);
    prevSiteEdge->setNext(nextSiteEdge);
    nextSiteEdge->setPrev(prevSiteEdge);
    
    //Handle the left side
    prevSiteEdge->getTwin()->setPrev(newEdge);
    newEdge->setNext(prevSiteEdge->getTwin());
    prevSiteEdge->getTwin()->setOrigin(center);
    
    //Handle the right side
    newEdge->getTwin()->setOrigin(center);
    newEdge->getTwin()->setPrev(nextSiteEdge->getTwin());
    nextSiteEdge->getTwin()->setNext(newEdge->getTwin());
    
    
    //Step3
    checkCircleEvent(leftSite, nullptr, leftSite->Y());
    checkCircleEvent(rightSite, nullptr, rightSite->Y());
    
    //delete the site from the tree
    mStatusTree.erase(*(event->node));
    event->setValid(false);
}

void VoronoiDiagram::finishEdge()
{
    double deepestSweep = bbMax.x + (bbMax.x-bbMin.x) + (bbMax.y-bbMin.y);
    
    for(auto iter = mStatusTree.begin(); iter != mStatusTree.end(); iter++)
    {
//        if(iter->edge)
            
    }
}

DCEL::Region* VoronoiDiagram::getRegion(DCEL::Vertex site)
{
    DCEL::Region* retVal = nullptr;
    
    auto iter = siteToRegion.find(site);
    
    if(iter == siteToRegion.end())
    {
        retVal = new DCEL::Region(site);
        siteToRegion.insert(std::pair<DCEL::Vertex, DCEL::Region*>(site,retVal));
    }
    else{
        retVal = iter->second;
    }
    
    return retVal;
}

void VoronoiDiagram::linkRegionToEdge(DCEL::Region* reg, DCEL::HalfEdge* edge)
{
    edge->setRegion(reg);
    
    if(!reg->getBound())
        reg->setBound(edge);
}

DCEL::Edge* VoronoiDiagram::getEdge(DCEL::Region *r1, DCEL::Region *r2)
{
    DCEL::Edge* retVal = nullptr;
    DCEL::Region* edgeR1 = nullptr;
    DCEL::Region* edgeR2 = nullptr;
    
    for(auto iter = edges.begin(); iter != edges.end() && retVal == nullptr; iter++)
    {
        edgeR1 = (*iter)->getRegion();
        edgeR2 = (*iter)->getTwin()->getRegion();
        
        if(edgeR1 == r1 && edgeR2 == r2)
        {
            retVal = *iter;
        }
        else if(edgeR1 == r2 && edgeR2 == r1)
        {
            retVal = (*iter)->getTwin();
        }
    }
    
    return retVal;
}

DCEL::Edge* VoronoiDiagram::createEdges(DCEL::Vertex* s1, DCEL::Vertex* s2, DCEL::Vertex breakpoint)
{
    DCEL::Region* r1 = getRegion(*s1);
    DCEL::Region* r2 = getRegion(*s2);
    DCEL::HalfEdge* e1 = new DCEL::HalfEdge();
    DCEL::HalfEdge* e2 = new DCEL::HalfEdge();

    edges.push_back(e1);
    
    e1->setTwin(e2);
    e1->setOrigin(new DCEL::Vertex(breakpoint));
    e1->setRegion(r1);
    
    e2->setOrigin(new DCEL::Vertex(breakpoint));
    e2->setRegion(r2);
    
    linkRegionToEdge(r1,e1);
    linkRegionToEdge(r2,e2);
    
    edges.push_back(e1);
    
    return e1;
}
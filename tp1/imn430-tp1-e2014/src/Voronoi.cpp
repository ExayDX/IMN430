//
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
    bbMin.x = bbMin.x > vertex.x ? vertex.x : bbMin.x;
    bbMin.y = bbMin.y > vertex.y ? vertex.y : bbMin.y;
    bbMax.x = bbMax.x < vertex.x ? vertex.x : bbMax.x;
    bbMax.y = bbMax.y < vertex.y ? vertex.y : bbMax.y;
    
    sites.insert(new DCEL::Vertex(vertex));
}


//void VoronoiDiagram::fortuneAlgorithm(const std::set<DCEL::Vertex*, DCEL::Vertex::Compare>& sites){
void VoronoiDiagram::fortuneAlgorithm(){
    vertices.clear();
    
    if(sites.empty()){
        return;
    }
    
    //Step1
    for(auto iter = sites.begin(); iter != sites.end(); iter++){
        mEventQueue.push(new SiteEvent(*iter));
    }
    
    //Step2
    VoronoiEvent* event = nullptr;
    while(!mEventQueue.empty()){
        event = mEventQueue.top();
        mEventQueue.pop();
        
        if(event->valid)
        {
            //Update fortune's line
            line = event->point->y;
        
            //Handle event
            if(event->isSiteEvent()){
                handleSiteEvent(event->point);
            }
            else{
                handleCircleEvent(reinterpret_cast<CircleEvent*>(event));
            }
        }
//FIXME: A REMETTRE
//        delete event;
    }
    
    //Step3
    finishEdge();
    
    //return edges;
}

/*
 Here we have the case where we have a double intersection at the same point
 like 2 horz point. In that case we need to figure out a way to solve that...
 !!!!!!!!!!!!
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
    i1->y = -(-c1 + dp1 / 4 + i1->x * i1->x / dp1);
    i2->y = -(-c2 + dp2 / 4 + i2->x * i2->x / dp2);
    
}

//void VoronoiDiagram::getIntersection(DCEL::Vertex* s1, DCEL::Vertex* s2, const double y)const
//{
//    DCEL::Vertex i1, i2;
//    
//    getIntersection(s1, s2, i1, i2, y);
//    
//    //Get parabola distance
//    double dp = 2.0 * (s1->y - y);
//    const double a1 = 1.0 / dp;
//    const double b1 = -2.0 * s1->x / dp;
//    const double c1 = y + dp / 4 + s1->x * s1->x / dp;
//    
//    //Get parabola distance
//    dp = 2.0 * (s2->y - y);
//    const double a2 = 1.0 / dp;
//    const double b2 = -2.0 * s2->x/dp;
//    const double c2 = y + dp / 4 + s2->x * s2->x / dp;
//    
//    //Compare Equation
//    const double a = a1 - a2;
//    const double b = b1 - b2;
//    const double c = c1 - c2;
//    
//    //Compute discriminant
//    const double disc = b*b - 4 * a * c;
//    const double _2_A = 2*a;
//    const double sqrt_disct = std::sqrt(disc);
//    
//    //Get point value
//    const double x1 = (-b + sqrt_disct) / _2_A;
//    const double x2 = (-b - sqrt_disct) / _2_A;
//    
//    //Getthe closest
//    return ((p->y < r->y) ? std::max(x1, x2) : std::min(x1, x2));
//    
//}

//FIXME: I guess we must handle the case were we have 2 points verticals and one to the side.
bool VoronoiDiagram::computeCircle(DCEL::Vertex v1, DCEL::Vertex v2,
                                   DCEL::Vertex v3, DCEL::Vertex& center,
                                   double& radius)
{
//    c->center.w = 1.0;
//    vertex *v1 = (vertex *)c->c.p1;
//    vertex *v2 = (vertex *)c->c.p2;
//    vertex *v3 = (vertex *)c->c.p3;
//    float bx = v1->xw; float by = v1->yw;
//    float cx = v2->xw; float cy = v2->yw;
//    float dx = v3->xw; float dy = v3->yw;
//    float temp = cx*cx+cy*cy;
//    float bc = (bx*bx + by*by - temp)/2.0;
//    float cd = (temp - dx*dx - dy*dy)/2.0;
//    float det = (bx-cx)*(cy-dy)-(cx-dx)*(by-cy);
//    if (fabs(det) < 1.0e-6) {
//        c->center.xw = c->center.yw = 1.0;
//        c->center.w = 0.0;
//        c->v1 = *v1;
//        c->v2 = *v2;
//        c->v3 = *v3;
//        return;
//    }
//    det = 1/det;
//    c->center.xw = (bc*(cy-dy)-cd*(by-cy))*det;
//    c->center.yw = ((bx-cx)*cd-(cx-dx)*bc)*det;
//    cx = c->center.xw; cy = c->center.yw;
//    c->radius = sqrt((cx-bx)*(cx-bx)+(cy-by)*(cy-by));
    
    //c->center.w = 1.0;
    bool valid = true;
    float bx = v1.x; float by = v1.y;
    float cx = v2.x; float cy = v2.y;
    float dx = v3.x; float dy = v3.y;
    float temp = cx*cx+cy*cy;
    float bc = (bx*bx + by*by - temp)/2.0;
    float cd = (temp - dx*dx - dy*dy)/2.0;
    float det = (bx-cx)*(cy-dy)-(cx-dx)*(by-cy);
    
    if (fabs(det) < 1.0e-6) {
        center.x = center.y = 1.0;
        valid = false;
    }
    else
    {
        det = 1/det;
        center.x = (bc*(cy-dy)-cd*(by-cy))*det;
        center.y = ((bx-cx)*cd-(cx-dx)*bc)*det;
        cx = center.x; cy = center.y;
        radius = sqrt((cx-bx)*(cx-bx)+(cy-by)*(cy-by));
    }
    return valid;
}

void VoronoiDiagram::addCircleEvent(VoronoiDiagram::status_type::iterator iter, bool isLeft)
{
    double radius = -1;
    DCEL::Vertex center;
    DCEL::Vertex first(iter->X(), iter->Y());
    
    //moving to the next  point
    isLeft ? iter-- : iter++;
    if (iter == mStatusTree.end()) //FIXME: to remove if it work
        return;
    
    DCEL::Vertex mid(iter->X(), iter->Y());
    
    isLeft ? iter-- : iter++;
    if (iter == mStatusTree.end()) //FIXME: to remove if it work
        return;
    
    DCEL::Vertex last(iter->X(), iter->Y());
    
    //TODO: compute the circle.
    if(computeCircle(first, mid, last, center, radius))
    {
        //TODO: add the circle event to the event queue and add the event to the tree
        std::cout << "center: " << center.x << " " << center.y << " " << " " << radius << " " <<  center.y + radius << std::endl;
    }
}

void VoronoiDiagram::handleSiteEvent(DCEL::Vertex* site){
    if(mStatusTree.empty())
    {
        mStatusTree.insert(tree_type(site, nullptr));
        return;
    }
    
    double minYInter = std::numeric_limits<double>::max();
    double yIntersec = minYInter;
    
//Step 2 check intersection with parabolas directly above pi
    status_type::iterator intersectedParabola = mStatusTree.begin();
    for(status_type::iterator i = mStatusTree.begin(); i != mStatusTree.end(); ++i){
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
    if(intersectedParabola->event != nullptr)
    {
        intersectedParabola->event->setValid(false);
    }
    
    //Step 3 insert the new site in the tree
    auto insertPos = (mStatusTree.insert(tree_type(site,nullptr)).first);
    
    //Step 4 edges
    //TODO: a terminer
    createEdges(site, intersectedParabola->site, minYInter);
//    DCEL::Edge* tmp = new DCEL::Edge(new DCEL::Vertex(site->x, minYInter));
//    tmp->setSite1(intersectedParabola->site);
//    tmp->setSite2(site);
//    
//    edges.push_back(DCEL::Edge(new DCEL::Vertex(site->x, minYInter)));
//    edges.push_back(tmp);
    
//Step 5 Create circles
    //the left one
    if(intersectedParabola != mStatusTree.begin())
    {
        checkCircleEvent(prev(intersectedParabola), &(*insertPos),site->x );
    }
    
    //the middle one
    checkCircleEvent(intersectedParabola, &(*insertPos), site->x );
    
    //the right one
    if(next(intersectedParabola) != mStatusTree.end())
    {
        checkCircleEvent(next(intersectedParabola), &(*insertPos), site->x);
    }
}

//code very lightly modifed from http://www.cs.hmc.edu/~mbrubeck/voronoi.html
bool VoronoiDiagram::circle(DCEL::Vertex p1, DCEL::Vertex p2, DCEL::Vertex p3, double *radius, DCEL::Vertex *center)
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

void VoronoiDiagram::checkCircleEvent(status_type::iterator parabola, const tree_type* node, double x)
{
    //If there's no site before or after, do not compute...
    if ((parabola == mStatusTree.begin()) || (parabola == std::prev(mStatusTree.end())))
        return;
    
    status_type::iterator tmp = prev(parabola);
    DCEL::Vertex a(tmp->X(), tmp->Y());
    DCEL::Vertex b(parabola->X(), parabola->Y());
    tmp = next(parabola);
    DCEL::Vertex c(tmp->X(), tmp->Y());
    
    DCEL::Vertex circleCenter;
    double circleRadius;
    
    if (circle(a, b, c, &circleRadius, &circleCenter) && (circleRadius > x))
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
//FIXME: to keep
//    if(!event->valid) //sanity check
//    {
//        return;
//    }
//    
//    //step1
//    //Fuck alex!!! et ses ITS
//    auto iter = mStatusTree.find(*(event->node));
//    if(iter != mStatusTree.begin() && prev(iter)->event != nullptr){
//        auto p = prev(iter);
//        p->event->setValid(false);
//        const_cast<tree_type*>(&(*p))->event = nullptr;
//    }
//    if(iter != mStatusTree.end() && next(iter)->event != nullptr){
//        auto n = next(iter);
//        n->event->setValid(false);
//        const_cast<tree_type*>(&(*n))->event = nullptr;
//    }
//    
//    //Step 2
//    vertices.push_back(event->mCenter);
//    //create 2 half-edge record corresponding to the new breakpoint on the beach line.
//    //set the pointer between them
//    //Attatch the 3 new record to the half-edge that end at the vertex
//    
//    
//    //delete the site from the tree
//    mStatusTree.erase(*(event->node));
//    event->setValid(false);
}

void VoronoiDiagram::checkCircle(tree_type* node, DCEL::Vertex* point){
//    assert(node && point);
//    
//    CircleEvent* event = node->event;
//    tree_type* left = node->getLeft();
//    tree_type* right = node->getRight();
//    
//    if(event && point != event->point){
//        event->setValid(false);
//    }
//    event = nullptr;
//    
//    if(left && right){
//        double y;
//        DCEL::Vertex* intersection = circle(
//            left->site,
//            node->site,
//            right->site,
//            &y
//        );
//        
//        if(intersection){
//            //node->event = new CircleEvent();
//        }
//    }
}

//DCEL::Vertex* VoronoiDiagram::circle(DCEL::Vertex* left, DCEL::Vertex* midlle, DCEL::Vertex* right, double* y_inter){
//    
//    //if(){}
//    
//    return nullptr;
//}
//
//

void VoronoiDiagram::finishEdge()
{
    double deepestSweep = bbMax.x + (bbMax.x-bbMin.x) + (bbMax.y-bbMin.y);
    
    for(auto iter = mStatusTree.begin(); iter != mStatusTree.end(); iter++)
    {
//        if(iter->edge)
            
    }
}

void VoronoiDiagram::createEdges(DCEL::Vertex* s1, DCEL::Vertex* s2, double sweepDepth)
{
    DCEL::HalfEdge* e1 = new DCEL::HalfEdge();
    DCEL::HalfEdge* e2 = new DCEL::HalfEdge();
    
    e1->setSite(s1);
    e1->setTwin(e2);
    
    e2->setSite(s2);
    e2->setTwin(e1);
    
    DCEL::Vertex i1, i2;
    getIntersection(s1, s2, &i1, &i2, sweepDepth-1);
    
    if(s1->y == s2->y)
    {
        //we need compute the middle point
    }
    else
    {
        DCEL::Vertex* siteMaxY = s1->y > s2->y ? s1 : s2;
        DCEL::Vertex* siteMinY = s1->y < s2->y ? s1 : s2;
        DCEL::Vertex* interMinX = i1.x < i2.x ? &i1 : &i2;
        DCEL::Vertex* interMaxX = i1.x > i2.x ? &i1 : &i2;
        
        e1->setOrigin(interMinX);
        e1->setSite(siteMaxY);
        //TODO:Il faut assoicier le edge a une region
        
        e2->setOrigin(interMaxX);
        e2->setSite(siteMinY);
        //TODO:Il faut assoicier le edge a une region
    }
}
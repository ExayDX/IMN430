//
//  Voronoi.h
//  IMN430-TP1
//
//  Created by François Bélanger on 2014-05-13.
//  Copyright (c) 2014 François Bélanger 94 245 437. All rights reserved.
//

#ifndef __IMN430_TP1__Voronoi__
#define __IMN430_TP1__Voronoi__

#include <map>
#include <set>
#include <queue>

#include "DCEL.h"
#include "Tree.h"

using std::vector;

class VoronoiDiagram{
private:
	class VoronoiEvent{
	private:
		//---- Members
		DCEL::Vertex* point;//Point where the event occurs
		bool valid;//Define if this event is valid (Circle Event can be invalid)

	protected:
		//---- Constructors
		VoronoiEvent(DCEL::Vertex* pt, const bool isValid = true)
			: point(pt), valid(isValid){
		}
	public:
		//---- Destructors
		virtual ~VoronoiEvent(){
			//FIXME: we don't delete the point here because it belong to the
			//          main...
			//delete point;
		}

		DCEL::Vertex* getPoint() const{
			return point; 
		}
        
        bool isValid()const { 
			return valid; 
		}

        void setValid(bool valid){
            this->valid = valid;
        }
        
		//---- Virtual Methods
		virtual bool isSiteEvent()const = 0;

        //---- Comparator Object
        struct EventPointCompareY{
			bool operator()(const VoronoiEvent* ev1, const VoronoiEvent* ev2) const{
				if (ev1->getPoint()->y != ev2->getPoint()->y)
					return ev1->getPoint()->y < ev2->getPoint()->y;
				else
					return ev1->getPoint()->x < ev2->getPoint()->x;
			}
        };
    };
    
    class SiteEvent : public VoronoiEvent{
    public:
        SiteEvent(DCEL::Vertex* pt);
        bool isSiteEvent()const;
    };

    class CircleEvent : public VoronoiEvent{
    public:
        CircleEvent(DCEL::Vertex* pt, DCEL::Vertex center, const TreeNode<DCEL::Edge, DCEL::Vertex, CircleEvent>* node);
        bool isSiteEvent()const;
        const TreeNode<DCEL::Edge, DCEL::Vertex, CircleEvent>* node;
        DCEL::Vertex mCenter;
    };
    
public:
    //---- Typedefs
    typedef TreeNode<DCEL::Edge, DCEL::Vertex, CircleEvent> tree_type;
    typedef std::set<tree_type, tree_type::Compare> status_type;
    
    //---- Constructors
    VoronoiDiagram(const double owidth = 500, const double oheigth = 500)
        : line(0), root(nullptr), width(owidth), height(oheigth) {
    }
    
    //---- Public Methods
    void fortuneAlgorithm(); //entry point
    void addSite(DCEL::Vertex vertex);
    const vector<DCEL::Vertex>& getVertices() {
		return vertices;
	}
    
private:
    //---- Private Methods
    void getIntersection(DCEL::Vertex* site1, DCEL::Vertex* site2, DCEL::Vertex* i1, DCEL::Vertex* i2, const double y)const;
    bool computeCircle(DCEL::Vertex p1, DCEL::Vertex p2, DCEL::Vertex p3, double *radius, DCEL::Vertex *center);
    void finishEdge();
    DCEL::Edge* createEdges(DCEL::Vertex* s1, DCEL::Vertex* s2, DCEL::Vertex breakpoint);
    
    void clearDiagram();
    DCEL::Region* getRegion(DCEL::Vertex site);
    void linkRegionToEdge(DCEL::Region* region, DCEL::HalfEdge* edge);
    DCEL::Edge* getEdge(DCEL::Region *r1, DCEL::Region *r2);
    
    //---- Event handlers
    void handleSiteEvent(DCEL::Vertex* point);
    void handleCircleEvent(VoronoiDiagram::CircleEvent* event);
    void checkCircleEvent(status_type::iterator site, const tree_type* node, double y);
    
    //---- Members
	double line;//sweep line
	double width;//Bouding width of the VD
	double height;//Bounding Heigth of the VD
    
	DCEL::Vertex bbMin;
    DCEL::Vertex bbMax;
	
	tree_type* root;//To maintain the topology of the beach line
	std::priority_queue<VoronoiEvent, std::vector<VoronoiEvent*>, VoronoiEvent::EventPointCompareY> mEventQueue;
    status_type mStatusTree;
    std::set<DCEL::Vertex*, DCEL::Vertex::CompareVertex> sites;
    std::map<DCEL::Vertex, DCEL::Region*> siteToRegion;
    
    //FIXME: dev test
public:
	std::vector<DCEL::Edge*> edges;
    
	//FIXME to remove
    //std::set<DCEL::Region*, DCEL::Region::CompareRegionY> regions;
    
	vector<DCEL::Vertex> vertices;//Keep track of the constructed VD
};
#endif /* defined(__IMN430_TP1__Voronoi__) */

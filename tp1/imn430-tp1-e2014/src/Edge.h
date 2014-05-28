//
//  Edge.h
//  IMN430_tp1
//
//  Created by Steven Aubertin on 2014-05-13.
//

#ifndef IMN430_tp1_Edge_h
#define IMN430_tp1_Edge_h

#include <cassert>

namespace DCEL 
{
    //---- Foward Declaration
    class Region;
    class Vertex;
    
    /*
        Edges are oriented counterclockwise inside each region
        Because an edge may border two regions an edge is replace by two half edge
     */
    class Edge
	{
    public:
        //---- Constructors
        Edge(Vertex* v)
            : prev(nullptr), next(nullptr), twin(nullptr), origin(v), left(nullptr){}
        
		Edge(Edge* oprev = nullptr, Edge* onext = nullptr)
            : prev(oprev), next(onext), twin(nullptr), origin(nullptr), left(nullptr){}
        
        //---- Accessors
        inline Edge* getTwin()const{ return this->twin;}

        inline Edge* getNext()const{ return this->next;}

        inline Edge* getPrev()const{ return this->prev;}

        inline Vertex* getOrigin()const{ return this->origin;}

        inline Region* getRegion()const{ return this->left;}
        
        //---- Mutators
        inline void setTwin(Edge* twin)
		{
            assert(twin);
            
            this->twin = twin;
            twin->twin = this;
        }

        inline void setNext(Edge* next)
		{
            assert(next);
            
            this->next = next;
            next->prev = this;
        }

        inline void setPrev(Edge* prev)
		{
            assert(prev);
            
            this->prev = prev;
            prev->next = this;
        }

        inline void setOrigin(Vertex* origin)
		{
            assert(origin);
            
            this->origin = origin;
        }

        inline void setRegion(Region* leftRegion)
		{
            assert(leftRegion);
            
            this->left = leftRegion;
        }
        
    private:
        //---- Members
        Edge* prev;
        Edge* next;
        Edge* twin;
        Vertex*   origin;
        Region*   left;
    };
    
    //---- An edge is a half edge
    typedef Edge HalfEdge;
}

#endif

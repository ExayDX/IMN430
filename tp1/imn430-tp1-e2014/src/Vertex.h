//
//  Vertex.h
//  IMN430_tp1
//
//  Created by Steven Aubertin on 2014-05-13.
//

#ifndef IMN430_tp1_Vertex_h
#define IMN430_tp1_Vertex_h

#include "EdgeIterator.h"
#include <memory>
#include <cmath>

namespace DCEL {
    /*
        Traversing a Region from a Vertex
        
        Given the edge e
        Where is origin is v
     
        start = e
        while next(twin(e)) != start
            e = next(twin(e))
     */
    class Vertex{
    public:
        //---- Constructors
        Vertex(HalfEdge* oedge = nullptr) :
			edge(oedge),   
			x(0),   
			y(0),   
			z(0){
        }
        
        Vertex(int xPos, int yPos, HalfEdge* oedge = nullptr, int zPos = 0) :
			edge(oedge),
			x(xPos), 
			y(yPos), 
			z(zPos){
        }
        
        Vertex(const Vertex& v) : 
			edge(v.edge), 
			x(v.x), 
			y(v.y), 
			z(v.z){
        }
        
        //---- Accessors
        inline HalfEdge* getEdge()const{
            return this->edge;
        }
        
        //---- Iterators
    private:
        class VertexIterator : public EdgeIterator{
        public:
            //---- Constructors
            VertexIterator(const Vertex* vertex): 
				EdgeIterator(vertex->getEdge()){
            }
            
            //---- Overriden Methods
            inline Edge* getNext(){
                Edge* n = this->next;
                
                if(this->hasNext()){
                    Edge* twin = this->next->getTwin();
                    this->next = twin->getNext();
                }
                
                if(this->isStart(this->next)){
                    this->next = nullptr;
                }
                
                return n;
            }
        };

    public:
        typedef VertexIterator iterator_type;
        iterator_type begin(){
            return iterator_type(this);
        }
        iterator_type end(){
            return iterator_type(nullptr);
        }
        
        //Compute distance between two Vertex (Eucledian distance)
        static double dist(const Vertex& v1, const Vertex& v2){
            const double dx = v1.x - v2.x;
            const double dy = v1.y - v2.y;
            return sqrt((dx * dx) + (dy * dy));
        }
        static double dist(const Vertex* v1, const Vertex* v2){
            return dist(*v1, *v2);
        }
        double dist(const Vertex& v)const{
            return dist(*this, v);
        }
        
        //---- Operators
        bool operator < (const Vertex& v){
            return y < v.y;
        }
        const bool operator < (const Vertex& v)const{
            return y < v.y;
        }
        bool operator > (const Vertex& v){
            return y > v.y;
        }
        const bool operator > (const Vertex& v)const{
            return y > v.y;
        }
        
        //---- Members
        HalfEdge* edge;
        
        int x;
        int y;
        int z;
        
        struct CompareVertex : public std::binary_function<Vertex*, Vertex*, bool>{
            bool operator()(const Vertex* v1, const Vertex* v2){
				if (v1->y != v2->y)
					return v1->y > v2->y;
				else
					return v1->x < v2->x; 
            }
        };
    };
    
    typedef std::shared_ptr<Vertex> vertex_ptr;
}

#endif

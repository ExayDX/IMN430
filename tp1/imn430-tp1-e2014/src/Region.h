//
//  Region.h
//  IMN430_tp1
//
//  Created by Steven Aubertin on 2014-05-13.
//  Copyright (c) 2014 Steven Aubertin. All rights reserved.
//

#ifndef IMN430_tp1_Region_h
#define IMN430_tp1_Region_h

#include "EdgeIterator.h"
#include <memory>
    using std::shared_ptr;


namespace DCEL 
{
    /*
        Region or face in a DCEL
     
        Traversing a Region
        for a giving edge e
     
        start = e
        while next(e) != start
            e = next(e)
     */
    class Region
	{
    public:
        //---- Constructors
        Region(HalfEdge* obound = nullptr) :
            bound(obound){}
        
        //---- Accessors
        inline HalfEdge* getBound() const
		{
            return bound;
        }
        
        //---- Mutators
        inline void setBound(HalfEdge* bound)
		{
            assert(bound);
            this->bound = bound;
        }
        
        //---- Iterators
    private:
        
		class RegionIterator : public EdgeIterator
		{
        public:
            //---- Constructors
            RegionIterator(const Region* region)
                : EdgeIterator(region->getBound()){}
            
            //---- Overriden Methods
            inline Edge* getNext()
			{
                Edge* n = this->next;
                
                //--- If not the end we go foward
                if(this->hasNext())
                    this->next = this->next->getNext();
                
                //--- If we are at the begining then it's the end
                if(this->isStart(this->next))
                    this->next = nullptr;
                
                return n;
            }
        };

    public:
        typedef RegionIterator iterator_type;
        iterator_type begin()
		{
            return iterator_type(this);
        }

        iterator_type end()
		{
            return iterator_type(nullptr);
        }
        
    private:
        //---- Members
        HalfEdge* bound;
        
        //TODO: If we need to add hole to a region it will be here
        //std::List<HalfEdge*> holes; ou quelque chose comme ca
    };
}

#endif

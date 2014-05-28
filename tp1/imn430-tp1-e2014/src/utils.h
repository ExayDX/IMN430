//
//  utils.h
//  IMN430-TP1
//
//  Created by François Bélanger on 2014-05-13.
//  Copyright (c) 2014 François Bélanger 94 245 437. All rights reserved.
//

#ifndef IMN430_TP1_utils_h
#define IMN430_TP1_utils_h

#include "DCEL.h"

namespace UTILS{
   class Site
    {
    public:
        Site(int xPos, int yPos) :
          x(xPos)
        , y(yPos)
        {}

        int x;
        int y;
        
    }; // class Site
}; //namespace UTILS

struct compareSite {
    bool operator() (const UTILS::Site& lhs, const UTILS::Site& rhs) const
    {return lhs.y<rhs.y;}
};


struct compareNode /*: public std::binary_function<DCEL::Vertex, DCEL::Vertex, bool>*/{
	bool operator()(const DCEL::Vertex* v1, const DCEL::Vertex* v2){
		if (v1->x == v2->x)
			return v1->y < v2->y;
		else
			return v1->x < v2->x;
	}
};

#endif

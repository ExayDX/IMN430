#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <vector>

#include "Vertex.h"
#include "vect.h"
#include <map>

class ConvexHull
{
	std::vector<DCEL::Vertex> pointList;
	std::vector<DCEL::Vertex> convexHullPoints;
	static const int NB_POINTS_LIMIT = 80; 
	std::map<DCEL::Vertex*, std::vector<DCEL::Region*> > Fconflit; 
	std::map<DCEL::Region*, std::vector<DCEL::Vertex*> > Pconflit; 



public :
	ConvexHull();
	ConvexHull(const char* filename); 
	~ConvexHull(); 

	bool loadFile(const char* filename); 
	void computeConvexHull();
	bool isCoplanar(const vect& ab, const vect& ac, const vect& ad);
};

#endif
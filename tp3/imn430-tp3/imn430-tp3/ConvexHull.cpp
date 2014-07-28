#include "ConvexHull.h"
#include "Region.h"

#include <algorithm>

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <map>
using std::map;

#include <fstream>
#include <string>
#include <sstream>
#include <cctype>


ConvexHull::ConvexHull()
{

}

ConvexHull::ConvexHull(const char* filename)
{
	if (!loadFile(filename))
		cout << "Unable to load file..." << endl;
}

ConvexHull::~ConvexHull(){}


bool ConvexHull::loadFile(const char* filename)
{
	bool success = false;

	if (filename)
	{
		pointList.clear();

		std::ifstream ifs(filename, std::ifstream::in);

		while (ifs.good() && !ifs.eof())
		{
			DCEL::Vertex pt;
			ifs >> pt;
			pointList.push_back(pt);
		}

		ifs.close();

		success = true;
	}

	return success;
}

bool ConvexHull::isCoplanar(const vect& ab, const vect& ac, const vect& ad){
	return abs(ab.cross(ac).dot(ad)) > 0;
}

void ConvexHull::computeConvexHull()
{

	std::vector<DCEL::Vertex> Clist; 

	if (pointList.size() >= 4)
	{
		//STEP 1
		//find 4 points not coplanar
		if (Clist.size() < 4)
		{
			auto aItt = pointList.begin();
			auto bItt = ++aItt;
			auto cItt = ++bItt;
			auto dItt = ++cItt;

			auto ab = vect(*aItt, *bItt);
			auto ac = vect(*aItt, *cItt);

			bool pointsFound = false;
			while (!pointsFound)
			{
				for (; dItt != pointList.end() && isCoplanar(ab, ac, vect(*aItt, *dItt)); ++dItt);
				if (dItt == pointList.end())
				{
					cout << "All points are coplanar. The convex hull can be computed with a 2D algorithm. Program is closing..." << endl;
					return;
				}
				else
				{
					//STEP 2
					//Keep (p1,p2,p3,p4) ( C <- CH(p1,p2,p3,p4))
					Clist.push_back(*aItt);
					Clist.push_back(*bItt);
					Clist.push_back(*cItt);
					Clist.push_back(*dItt);
					//Not sure if we should erase those points.... :( 
					pointList.erase(aItt);
					pointList.erase(bItt);
					pointList.erase(cItt);
					pointList.erase(dItt);
				}
			}

			//STEP 3
			//Shuffle rests of the points
			std::random_shuffle(pointList.begin(), pointList.end());

			//STEP 4
			//Initalize the conflits Graph<aNpoint, aCface>
			//FRANK HERE!!!!
			std::map<DCEL::Vertex, DCEL::Region> conflictsGraph;

			//for (auto it = pointList.begin(); it != pointList.end(); ++it)
			//{
			//	conflictsGraph.insert(std::pair<DCEL::Vertex, DCEL::Region>(*it, ));
			//}

			//STEP 5
			for (int i = 5; i < pointList.size(); ++i)
			{

			}
		}
		//else
			//Changer (voir diapo 154)
			
	}
	else
		cout << "Not enough points to create a convex hull, program is closing..." << endl;
}

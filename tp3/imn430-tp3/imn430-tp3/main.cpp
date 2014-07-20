//
//  main.cpp
//  imn430-tp3
//
//  Created by Steven Aubertin on 2014-07-16.
//

#include <algorithm>

#include <fstream>
using std::ifstream;

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <map>
using std::map;

#include <vector>
using std::vector;

#include "Region.h"
#include "vect.h"
#include "Vertex.h"

bool load_file(const char* filename, vector<DCEL::Vertex>& N){
    bool success = false;
    
    if(filename){
        N.clear();
        
        std::ifstream ifs(filename, std::ifstream::in);
        
        while(ifs.good() && !ifs.eof()){
            DCEL::Vertex pt;
            ifs >> pt;
            N.push_back(pt);
        }
        
        ifs.close();
        
        success = true;
    }
    
    return success;
}

//template<class T>
bool is_coplanar(const vect& ab, const vect& ac, const vect& ad){
   return abs(ab.cross(ac).dot(ad)) > 0;
}

int main(int argc, const char * argv[]){
    vector<DCEL::Vertex> N;
    
    if(load_file("ananas.txt", N)){
        cout << N.size() << endl;
        
        //STEP 1
        //find 4 points not coplanar
        if(N.size() >= 4){
            //points that create our tetraedon
            auto aItt = N.begin();
            auto bItt = next(aItt);
            auto cItt = next(bItt);
            auto dItt = next(cItt);
            
            auto ab = vect(*aItt, *bItt);
            auto ac = vect(*aItt, *cItt);
            for(; dItt != N.end() && is_coplanar(ab, ac, vect(*aItt, *dItt)); ++dItt);
            
			if (dItt != N.end()){
				//STEP 2
				//Keep (p1,p2,p3,p4) ( C <- CH(p1,p2,p3,p4))
				auto a = *aItt;
				auto b = *bItt;
				auto c = *cItt;
				auto d = *dItt;

				N.erase(aItt);
				N.erase(bItt);
				N.erase(cItt);
				N.erase(dItt);

				//STEP 3
				//Shuffle rests of the points
				std::random_shuffle(N.begin(), N.end());

				//STEP 4
				//Initalize the conflits Graph<aNpoint, aCface>
				//VERIFY : second type is probably wrong (will affect next loop)
				std::map<DCEL::Vertex, DCEL::Region> conflictsGraph;

				/*for (auto it = N.begin(); it != N.end(); ++it)
				{
					conflictsGraph.insert(std::pair<point3d<double>, point3d<double>>(*it, a)); 
					conflictsGraph.insert(std::pair<point3d<double>, point3d<double>>(*it, b));
					conflictsGraph.insert(std::pair<point3d<double>, point3d<double>>(*it, c));
					conflictsGraph.insert(std::pair<point3d<double>, point3d<double>>(*it, d));
				}
*/
				//STEP 5
				for (int i = 5; i < N.size(); ++i)
				{

				}
            }
            else{
				//Changer (voir diapo 154)
                cout << "Unable to find 4 points that are not coplanar, program is closing..." << endl;
            }
        }
        else{
            cout << "Not enough points to create a convex hull, program is closing..." << endl;
        }
    }
    else{
        cout << "Unable to load file, program is closing..." << endl;
    }
    
    return 0;
}


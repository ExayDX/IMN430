//
//  main.cpp
//  imn430-tp3
//
//  Created by Steven Aubertin on 2014-07-16.
//

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include "point3d.h"
#include "vect.h"

bool load_file(const char* filename, vector<point3d<double>>& N){
    bool success = false;
    
    if(filename){
        N.clear();
        
        std::ifstream ifs(filename, std::ifstream::in);
        
        while(ifs.good() && !ifs.eof()){
            point3d<double> pt;
            ifs >> pt;
            N.push_back(pt);
        }
        
        ifs.close();
        
        success = true;
    }
    
    return success;
}

template<class T>
bool is_coplanar(const vect<T>& ab, const vect<T>& ac, const vect<T>& ad){
    return abs(ab.cross(ac).dot(ad)) > 0;
}

int main(int argc, const char * argv[]){
    vector<point3d<double>> N;
    
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
            
            auto ab = vect<double>(*aItt, *bItt);
            auto ac = vect<double>(*aItt, *cItt);
            for(; dItt != N.end() && is_coplanar(ab, ac, vect<double>(*aItt, *dItt)); ++dItt);
            
            if(dItt != N.end()){
                auto a = *aItt;
                auto b = *bItt;
                auto c = *cItt;
                auto d = *dItt;
                
                N.erase(aItt);
                N.erase(bItt);
                N.erase(cItt);
                N.erase(dItt);
                
                //STEP 2
                //Shuffle rests of the points
                std::random_shuffle(N.begin(), N.end());
            }
            else{
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


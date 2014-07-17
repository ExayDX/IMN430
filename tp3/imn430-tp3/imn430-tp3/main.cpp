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
            auto i = N.begin();
            //points that create our tetraedon
            point3d<double>* a = &(*i++);
            point3d<double>* b = &(*i++);
            point3d<double>* c = &(*i++);
            point3d<double>* d = nullptr;
            
            auto ab = vect<double>(*a, *b);
            auto ac = vect<double>(*a, *c);
            
            for(; i != N.end() && !d; ++i){
                d = &(*i++);
                
                auto ad = vect<double>(*a, *d);
                
                if(is_coplanar(ab, ac, ad)){
                    d = nullptr;
                }
            }
            if(d){
                cout << "YEAH BABY"<<endl;
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


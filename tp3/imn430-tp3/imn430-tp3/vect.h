//
//  vect.h
//  imn430-tp3
//
//  Created by Steven Aubertin on 2014-07-16.
//

#ifndef imn430_tp3_vect_h
#define imn430_tp3_vect_h

#include <iostream>
using std::istream;
using std::ostream;

#include "point3d.h"

template<class T>
class vect : public point3d<T>{
public:
    //---- typedefs
    typedef typename point3d<T>::value_type value_type;
    
    //---- Ctor
    vect(const value_type& x, const value_type& y, const value_type& z)
        : point3d<T>(x, y, z){
    }
    vect(const point3d<T>& a, const point3d<T>& b)
        : point3d<T>(b.x - a.x, b.y - a.y, b.z - a.z){
    }

    //---- Public methods
    vect cross(const vect& b)const{
        return vect(
            (this->y * b.z) - (this->z * b.y),
            (this->x * b.z) - (this->z * b.x),
            (this->x * b.y) - (this->y * b.x)
        );
    }
    
    value_type dot(const vect& v)const{
        return (this->x * v.x + this->y * v.y + this->z * v.z);
    }
};

#endif

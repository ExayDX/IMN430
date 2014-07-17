//
//  point3d.h
//  imn430-tp3
//
//  Created by Steven Aubertin on 2014-07-16.
//

#ifndef imn430_tp3_point3d_h
#define imn430_tp3_point3d_h

#include <iostream>
using std::istream;
using std::ostream;

template<class T>
class point3d{
public:
    //---- Typdefs
    typedef T value_type;
    
    //---- Ctor
    point3d(const value_type xo = 0, const value_type yo = 0, const value_type zo = 0)
        : x(xo), y(yo), z(zo){
    }
    point3d(const point3d& pt)
        : x(pt.x), y(pt.y), z(pt.z){
    }
    
    //---- Operators
    friend istream& operator >> (istream& is, point3d& p){
        if(is.good() && !is.eof()){
            is >> p.x;
            is >> p.y;
            is >> p.z;
        }
        
        return is;
    }
    
    friend ostream& operator << (ostream& os, const point3d& p){
        if(os.good()){
            os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
        }
        
        return os;
    }
    
    point3d& operator =(const point3d& pt){
        if(&pt != this){
            x = pt.x;
            y = pt.y;
            z = pt.z;
        }

        return *this;
    }
    
    point3d& operator *= (const value_type& scalar){
        x *= scalar;
        y *= scalar;
        z *= scalar;
        
        return *this;
    }
    const point3d& operator * (const value_type& scalar)const{
        return point3d(x * scalar, y * scalar, z * scalar);
    }
    
    //---- Members
    value_type x;
    value_type y;
    value_type z;
};

#endif

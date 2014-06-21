//
//  PCA.h
//  IMN430 TP2 - PCA
//

#ifndef PCA_h
#define PCA_h

#include <vector>
#include <cassert>
#include <string>
    using std::string;

#include "Cimg.h"
using namespace cimg_library;

class PCA
{
public :
    typedef float data_type;
    typedef CImg<data_type> image_type;
    typedef CImgList<data_type> images_list_type;
    
    //Lazy point
    typedef double coord_value;
    typedef std::pair<coord_value , coord_value> point;
#define X first
#define Y second
    
    //Lazy color
    enum{
        RED,
        GREEN,
        BLUE,
        
        GRAY = RED
    };
    typedef std::tuple<data_type, data_type, data_type> Color;
    
    
    //Lazy Pixel
    typedef std::pair<point, Color> pixel;
#define pt first
#define color second
    
	PCA();
	~PCA();
	
    void clear(){
        delete m_pVectorPoint;
        m_pVectorPoint = nullptr;
    }
    
	void GetPCA(CImg<float>* image, float threshold, CImg<float>& eigenValues, CImg<float>& eigenVectors);
    
    template<class T>
    static inline T clamp(const T val, const T min, const T max){
        return std::max(min, std::min(val, max));
    }
    
    image_type& image(const int i)const{
        return m_pVectorPoint->operator()(i);
    }
    image_type& GetX()const{
        return image(0);
    }
    image_type& GetY()const{
        return image(1);
    }
    image_type GetD()const{
        auto X1 = GetX();
        auto X2 = GetY();
        
        image_type D(2, static_cast<unsigned int>(X1.size()), X1.depth(), 1);
        
        for(auto i = 0; i < X1.size(); ++i){
            D(0,i,0,GRAY) = X1(i,0,0,GRAY);
        }
        for(auto i = 0; i < X2.size(); ++i){
            D(i,0,0,GRAY) = X2(i,0,0,GRAY);
        }
        
        return D;
    }
    
    template<class T=float, class DataVector=std::vector<T>>
    static T mean(const DataVector& x){
        T acc = 0;
        
        for(auto i = x.begin(); i < x.end(); ++i){
            acc += (*i);
        }
        
        return acc / x.size();
    }
    
    template<class T=float, class DataVector=std::vector<T>>
    static T variance(const DataVector& x){
        const T mean_value = mean(x);
        
        T acc = 0;
        for(auto i = x.begin(); i < x.end(); ++i){
            const T val = ((*i) - mean_value);
            acc += (val * val);
        }
        
        return acc / (x.size() - 1);
    }
    
    template<class T=float, class DataVector=std::vector<T>>
    static T covariance(const DataVector& x1, const DataVector& x2){
        assert(x1.size() == x2.size());
        
        const size_t SIZE = x1.size();
        const T mean_x1 = mean(x1);
        const T mean_x2 = mean(x2);
        
        T acc = 0;
        for(auto i = 0; i < SIZE; ++i){
            acc += ((x1[i] - mean_x1) * (x2[i] - mean_x2));
        }
        
        return acc / (SIZE - 1);
    }
    
    template<class T=float, class DataVector=std::vector<T>>
    static void NormaliseColumn(DataVector& m, const unsigned int column = 0){
        T norm = 0;
        for(auto i = 0; i < m.height(); ++i){
            norm += (m(i, column) * m(i,column));
        }
        norm = sqrt(norm);
        
        for(auto i = 0; i < m.height(); ++i){
            m(i,column) /= norm;
        }
    }
    template<class T=float, class DataVector=std::vector<T>>
    static void NormaliseLine(DataVector& m, const unsigned int line = 0){
        T norm = 0;
        for(auto i = 0; i < m.width(); ++i){
            norm += (m(line, i) * m(line, i));
        }
        norm = sqrt(norm);
        
        for(auto i = 0; i < m.width(); ++i){
            m(line,i) /= norm;
        }
    }
    
    template<class T=float, class DataVector=std::vector<T>>
    static void compute_eigenValues_n_vector(const DataVector& S, DataVector& eValues, DataVector& eVectors){
        const T a = S(0,0);
        const T b = S(0,1);
        const T c = S(1,0);
        const T d = S(1,1);
        
        //Pre computed constants
        const T a_2 = a * a;
        const T _2ad = 2.0 * (a * d);
        const T _4bc = 4.0 * (b * c);
        const T d_2 = d * d;
        const T a_d = a + d;
        
        const T _2_c = 2.0 * c;
        const T mina_d = -a + d;
        
        const T pre_computed = sqrt(a_2 - _2ad + _4bc + d_2);
        
        //Compute eigen values
        eValues(0,0) = 0.5 * (pre_computed + a_d);
        eValues(0,1) = 0.5 * (-pre_computed + a_d);
        
        //Compute eigen vectors
        eVectors(0,0) = -((mina_d - pre_computed) / (_2_c));
        eVectors(1,0) = 1;
        
        eVectors(0,1) = -((mina_d + pre_computed) / (_2_c));
        eVectors(1,1) = 1;
        
        //Normalise column vectors
        for(int i = 0; i < eVectors.width(); ++i){
            NormaliseColumn(eVectors, i);
        }
    }
    
//private:TODO remove that
    void GetCovarianceMatrix(CImg<float>& sMatrix);
    void CenterData(CImg<float>* image, float threshold);
    
    CImgList<float>* m_pVectorPoint;
    data_type meanColor;
};


#endif

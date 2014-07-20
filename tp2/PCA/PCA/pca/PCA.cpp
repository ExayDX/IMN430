//
//  PCA.cpp
//  IMN430 TP2 - PCA
//

#include "PCA.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace cimg_library::cimg;

PCA::PCA()
    : m_pVectorPoint(new images_list_type()), centroid(0,0){
}

PCA::~PCA(){
    clear();
}

void PCA::GetPCA(CImg<float>* image, float threshold, CImg<float>& eigenValues, CImg<float>& eigenVectors){
    if(m_pVectorPoint->size() > 0){
        m_pVectorPoint->clear();
    }
    
    // Center data
    CenterData(image, threshold);
    
    // Compute variance-covariance matrix (S matrix)
    CImg<float> S(m_pVectorPoint->size(), m_pVectorPoint->size(), image->depth(), 1);
    GetCovarianceMatrix(S);

    // Compute eigen values and eigen vectors of S-Matrix
    compute_eigenValues_n_vector(S, eigenValues, eigenVectors);
}

void PCA::CenterData(CImg<float>* image, float threshold){
    std::vector<pixel> data;
    
    //Get coords of points above the threshold
    centroid.X = 0;
    centroid.Y = 0;
    for(int i = 0; i != image->width(); ++i){
        for(int j = 0; j < image->height(); ++j){
            const data_type r = image->operator()(i,j,0, RED);
            const data_type g = image->operator()(i,j,0, GREEN);
            const data_type b = image->operator()(i,j,0, BLUE);
            
            //Y = 0.299*R + 0.587*G + 0.114*B
            const data_type gray = ((0.299 * r) + (0.587 * g) + (0.114 * b));
            
            if(gray >= threshold){
                data.push_back(pixel(point(i,j), Color(r,g,b)));
                centroid.X += i;
                centroid.Y += j;
            }
        }
    }
    
    //Create data(image) vectors
    const unsigned int SIZE = static_cast<unsigned int>(data.size());
    CImg<float> Xvector(1, SIZE, image->depth(), 1);
    CImg<float> Yvector(1, SIZE, image->depth(), 1);
    
    //Get the centroid values
    centroid.X /= static_cast<coord_value>(SIZE);
    centroid.Y /= static_cast<coord_value>(SIZE);
    
    //Translate data
    int xi = 0;
    for(auto i = begin(data); i != end(data); ++i, ++xi){
        Xvector(0, xi, 0, GRAY) = (i->pt.X - centroid.X);
        Yvector(0, xi, 0, GRAY) = (i->pt.Y - centroid.Y);
    }
    
    //Add the computed centered data vectors
    m_pVectorPoint->insert(Xvector);
    m_pVectorPoint->insert(Yvector);

#ifdef DEBUG
    CImg<float> outImg(image->width(), image->height(), image->depth(), 1);
    for(auto i = outImg.begin(); i != outImg.end(); ++i){
        *i = 0;
    }
    auto halfW = image->width() / 2;
    auto halfH = image->height() / 2;
    for(auto i = Xvector.begin(), j = Yvector.begin(); i != Xvector.end(); ++i, ++j){
        auto x = clamp(static_cast<typeof(image->width())>(*i) + halfW, 0, image->width());
        auto y = clamp(static_cast<typeof(image->height())>(*j) + halfH, 0, image->height());
        outImg(x, y, 0, GRAY) = 255;
    }
    outImg.save("center.ppm");
#endif
}

void PCA::GetCovarianceMatrix(CImg<float>& sMatrix){
    //all vector should have the same amount of data
    //this is checked in the variance function
    
    for(auto i = 0; i < m_pVectorPoint->size(); i++){
        for(auto j = 0; j < m_pVectorPoint->size(); ++j){
            
            auto Xi = m_pVectorPoint->operator()(i);
            auto Xj = m_pVectorPoint->operator()(j);
            
            sMatrix(i, j, 0, GRAY) = (i == j ? variance(Xi) : covariance(Xi, Xj));
        }
    }
}






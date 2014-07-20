//
//  PCA.cpp
//  IMN430 TP2 - PCA
//

#include "PCA.h"
#include <iostream>

using namespace cimg_library::cimg;

PCA::PCA()
{
    m_pVectorPoint = new CImgList<float>();
}

PCA::~PCA()
{
    delete m_pVectorPoint;
    m_pVectorPoint = 0;
}

void PCA::GetPCA(CImg<float>* image, float threshold, CImg<float>& eigenValues, CImg<float>& eigenVectors)
{
    if(m_pVectorPoint->size() > 0)
    {
        m_pVectorPoint->clear();
    }
    
    // Center data
    
    // Compute variance-covariance matrix (S matrix)

    // Compute eigen values and eigen vectors of S-Matrix
}

void PCA::CenterData(CImg<float>* image, float threshold)
{}

void PCA::GetCovarianceMatrix(CImg<float>& sMatrix)
{}
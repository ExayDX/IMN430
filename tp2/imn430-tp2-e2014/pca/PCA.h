//
//  PCA.h
//  IMN430 TP2 - PCA
//

#ifndef PCA_h
#define PCA_h

#include "Cimg.h"
using namespace cimg_library;

class PCA
{
public :
	PCA();
	~PCA();
	
	void GetPCA(CImg<float>* image, float threshold, CImg<float>& eigenValues, CImg<float>& eigenVectors);
    
private:
    void GetCovarianceMatrix(CImg<float>& sMatrix);
    void CenterData(CImg<float>* image, float threshold);
    
    CImgList<float>* m_pVectorPoint;
};


#endif

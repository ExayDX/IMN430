//
//  main.cpp
//  IMN430 TP2 - PCA
//

#include "Cimg.h"
#include "PCA.h"
#include <iostream>
    using std::cout;
    using std::endl;
    using std::cin;
#include <math.h>
#include <string>
    using std::string;
#include <vector>
    using std::vector;
#include <sstream>
    using std::stringstream;
#include <cassert>

template<class T = float>
bool loadImages(double& threashold, CImgList<T>& images, const size_t images_count = 2){
    enum eInput{
        eOvalInput  = 'o',
        eBrainInput = 'b',
        eQuitInput  = 'q',
        
        NB_INPUTS = 3
    };
    
    bool success = true;
    bool  quit  = false;
    char  input = NB_INPUTS;//Default
    const char* file  = nullptr;
    
    do{ 
        cout << "Please specify the type of images to load, or q to quit" << endl
             << "Align Ovals  : " << static_cast<char>(eOvalInput)  << endl
             << "Align Brains : " << static_cast<char>(eBrainInput) << endl
             << "> ";
    
        cin >> input;
    }
    while(!(quit = (input == eQuitInput)) && input != eOvalInput && input != eBrainInput);
    
    if(!quit){
        cout << "Please specify a threashold " << endl
             << "> ";
        cin >> threashold;
        
        switch(static_cast<eInput>(input)){
            case eOvalInput:
                file = "oval";
                break;
            case eBrainInput:
                file = "brain";
                break;
            default:
                //ERROR
                assert(false);
                break;
        }
        cout << "Loading images" << endl;
        for(auto i = 1; i <= images_count; ++i){
            stringstream ss;
            ss << file;  //file (oval or brain)
            ss << i;     //file no
            ss << ".ppm";//extension
            
            CImg<T> img;
            img.load(ss.str().c_str());
            if(images.insert(img) != images.end()){
                cout << "File : " << ss.str() << " loaded" << endl;
            }
            else{
                cout << "Unable to load image : " << ss.str() << endl;
                success = false;
                break;
            }
        }
        cout << endl;
    }
    
    return success;
}

unsigned long PosOfMaxValue(const CImg<float>& img){
    auto pos = 0;
    float maxValue = img(0,0,0,PCA::GRAY);
    for(auto i = 0; i < img.width(); ++i){
        if(maxValue < img(i,0,0,PCA::GRAY)){
            pos = i;
        }
    }
    return pos;
}

template<class T>
void display(const CImg<T>& img){
    for(auto i = 0; i < img.width(); ++i){
        for(auto j = 0; j < img.height(); ++j){
            cout << img(i,j,0,0) << "      ";
        }
        cout<<endl;
    }
    cout<<endl;
}

template<class T=float>
CImg<T> GetD(const CImg<T>& X1, const CImg<T>& X2){
    CImg<T> D(static_cast<unsigned int>(X1.size()), 2, 1, 1);
    
    auto i = 0;
    for(auto x = X1.begin(); x != X1.end(); ++x){
        D(i++,0,0,0) = *x;
    }
    
    i = 0;
    for(auto y = X2.begin(); y != X2.end(); ++y){
        D(i++,1,0,0) = *y;
    }
    
    return D;
}

template<class T=float>
T ComputeNorm(const CImg<T>& v){
    T norm = 0;
    for(auto i = v.begin(); i != v.end(); ++i){
        norm += ((*i) * (*i));
    }
    norm = sqrt(norm);
    
    return norm;
}

template<class T=float>
CImg<T> RotationMatrix(const CImg<T>& F, const CImg<T>& V){
    const double theta = acos(V.dot(F) / ComputeNorm(F));
    const double cosT = cos(theta);
    const double sinT = sin(theta);
    
    auto R = CImg<float>(2,2,1,1);
    R(0,0,0,0) = cosT;
    R(0,1,0,0) = -sinT;
    R(1,0,0,0) = sinT;
    R(1,1,0,0) = cosT;
    
    return R;
}

template<class T=float>
CImg<T> GetFt(const CImg<T>& eValues, const CImg<T>& eVectors){
    auto pos = PosOfMaxValue(eValues);
    
    auto F = CImg<float>(1,2,1,1);
    F(0,0,0,0) = eVectors(0, static_cast<unsigned int>(pos));
    F(0,1,0,0) = eVectors(1, static_cast<unsigned int>(pos));
    
    return F;
}

template<class T=float>
CImg<T> HomoDp(const CImg<T>& Dp){
    auto Dph = CImg<T>(Dp.size(), 2,1,1);
    auto j = 0;
    for(auto i = Dp.begin(); i != Dp.end(); ++i, ++j){
        Dph(j,0,0,0) = (*i);
        Dph(j,1,0,0) = 1;
    }
    return Dph;
}

//#undef DEBUG_EXERCICE

int main(int argc, char** argv){
#ifdef DEBUG_EXERCICE
    {
        CImg<float> Xtest(1,10,1,1);
        CImg<float> Ytest(1,10,1,1);
        
        Xtest(0,0,0,PCA::GRAY)=2.5;
        Xtest(0,1,0,PCA::GRAY)=0.5;
        Xtest(0,2,0,PCA::GRAY)=2.2;
        Xtest(0,3,0,PCA::GRAY)=1.9;
        Xtest(0,4,0,PCA::GRAY)=3.1;
        Xtest(0,5,0,PCA::GRAY)=2.3;
        Xtest(0,6,0,PCA::GRAY)=2.0;
        Xtest(0,7,0,PCA::GRAY)=1.0;
        Xtest(0,8,0,PCA::GRAY)=1.5;
        Xtest(0,9,0,PCA::GRAY)=1.1;
        
        Ytest(0,0,0,PCA::GRAY)=2.4;
        Ytest(0,1,0,PCA::GRAY)=0.7;
        Ytest(0,2,0,PCA::GRAY)=2.9;
        Ytest(0,3,0,PCA::GRAY)=2.2;
        Ytest(0,4,0,PCA::GRAY)=3.0;
        Ytest(0,5,0,PCA::GRAY)=2.7;
        Ytest(0,6,0,PCA::GRAY)=1.6;
        Ytest(0,7,0,PCA::GRAY)=1.1;
        Ytest(0,8,0,PCA::GRAY)=1.6;
        Ytest(0,9,0,PCA::GRAY)=0.9;
        
        auto xmean = PCA::mean(Xtest);
        auto ymean = PCA::mean(Ytest);
        cout<<"Mean vector : ("<< xmean << ", "<< ymean <<")"<<endl;
        
        for(auto i = Xtest.begin(); i != Xtest.end(); ++i){
            *i -= xmean;
        }
        for(auto i = Ytest.begin(); i != Ytest.end(); ++i){
            *i -= ymean;
        }
        
        cout<<"X"<<" | "<<"Y"<<endl
            <<"--------------"<<endl;
        for(auto Xi = Xtest.begin(), Yi = Ytest.begin(); Xi != Xtest.end(); ++Xi, ++Yi){
            cout<<*Xi<<" | "<<*Yi
                <<endl;
        }
        cout << endl;
        
        CImg<float> S(2,2, 1, 1);
        PCA pca;
        pca.m_pVectorPoint->insert(Xtest);
        pca.m_pVectorPoint->insert(Ytest);
        
        pca.GetCovarianceMatrix(S);
        cout<<"Covariance Matrix S"<<endl;
        display(S);
        
        CImg<float> eValues(1,2,pca.image(0).depth(),1);
        CImg<float> eVectors(2,2,pca.image(0).depth(),1);
        pca.compute_eigenValues_n_vector(S, eValues, eVectors);
        
        cout<<"Eigen vectors"<<endl;
        display(eVectors);
        
        cout << "Eigen values"<< endl;
        display(eValues);
        
        //GET  D
        auto D = GetD(Xtest, Ytest);
        cout<<"D : "<<endl;
        display(D);
        
        //GET Ft
        auto F = GetFt(eValues, eVectors);
        cout<<"Ft : "<<endl;
        display(F);
        
        auto Dt = D.get_transpose();
        cout<<"Dt"<<endl;
        display(Dt);
        
        auto Dp = Dt * F;
        cout << "Dprime" << endl;
        display(Dp);
        
        auto R = RotationMatrix(F);
        display(R);
        
        auto Dph = HomoDp(Dp);
        
        auto rotate = R * Dph;
        display(rotate);
        
        cout<<endl;
        exit(0);
#endif
    CImgList<float> images;
    double threashold = 0;
        
    if(loadImages(threashold, images)){
        for(auto i = 0; i < images.size(); ++i){
            PCA pca;
            CImg<float> eigenValues(1,2, images(i).depth(),1);
            CImg<float> eigenVectors(2,2,images(i).depth(),1);
            pca.GetPCA(&images(i), threashold, eigenValues, eigenVectors);
            auto Ft = GetFt(eigenValues, eigenVectors);
            
            auto V = CImg<float>(1,2,1,1);
            V(0,0,0,0) = -1;
            V(1,0,0,0) = 0;
            
            stringstream ss;
            ss << "out" << i << ".ppm";
            images(i).rotate((180 * acos(V.dot(Ft) / ComputeNorm(Ft)))/ cimg::PI).save(ss.str().c_str());
        }
    }
    
    return 0;
}







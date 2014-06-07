//
//  AcceBase.h
//  TextureSynthesis
//
//  Created by slgu on 5/5/14.
//  Copyright (c) 2014 gsl. All rights reserved.
//

#ifndef __TextureSynthesis__AcceBase__
#define __TextureSynthesis__AcceBase__

#include "common.h"
class AcceBase{
public:
    virtual pair < vector <double> , int >  query(const vector <double> & po) = 0;
protected:
    typedef vector < pair <vector <double>, int > > PointCloud;
    typedef pair <vector <double>, int > LabelPoint;
    static double dis(const vector <double> & a, const vector <double> & b){
        double res = 0;
        for(int i = 0; i < a.size(); ++i){
            res += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return res;
    }
    static void add(vector <double> & a, const vector <double> &b){
        for(int i = 0; i < a.size(); ++i){
            a[i] += b[i];
        }
    }
    static void div(vector <double> & a, const int & n){
        for(int i = 0; i < a.size(); ++i)
            a[i] /= n;
    }
};
#endif /* defined(__TextureSynthesis__AcceBase__) */

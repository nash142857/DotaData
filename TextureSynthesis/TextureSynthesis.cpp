//
//  TextureSynthesis.cpp
//  TextureSynthesis
//
//  Created by slgu on 4/28/14.
//  Copyright (c) 2014 gsl. All rights reserved.
//

#include <iostream>
#include "TextureSynthesis.h"
#include "AcceBase.h"
#include "KdTree.h"
#include "Tsvq.h"
#include <utility>
using namespace std;
extern const string basedir;
Mat TextureSynthesis::change(const string & str, int coe, int neighborsize, Datastruct type){
    Mat sample;
    sample = imread(str + ".jpg");
    int row = sample.rows;
    int col = sample.cols;
    int dimension = (neighborsize * neighborsize - 1) / 2 * 3;
    int offset = (neighborsize - 1) / 2;
    double * samplepixel = new double[3 * row * col];
    {
        int index = 0;
        for(auto i = sample.begin<Vec3b>(); i != sample.end<Vec3b>(); ++i){
            samplepixel[index++] = (*i)[0];
            samplepixel[index++] = (*i)[1];
            samplepixel[index++] = (*i)[2];
        }
    }
    vector < pair < vector < double > , int > > treedata;
    for(int i = 0; i < row; ++i)
        for(int j = 0; j < col; ++j){
            if( ((i - offset) < 0) || ((j - offset) < 0 ) ||(j + offset >= col))
                continue;
            vector <double> tmpvt;
            int tmpid = i * col + j;
            for(int k1 = -offset; k1 <= offset; ++k1){
                for(int k2 = -offset; k2 <= offset; ++ k2){
                    if(k1 == 0 && k2 == 0)
                        break;
                    int x = i + k1;
                    int y = j + k2;
                    tmpvt.push_back(samplepixel[3 * (x * col + y) + 0]);
                    tmpvt.push_back(samplepixel[3 * (x * col + y) + 1]);
                    tmpvt.push_back(samplepixel[3 * (x * col + y) + 2]);
                }
                if(k1 == 0)
                    break;
            }
            treedata.push_back(make_pair(tmpvt, tmpid));
        }
    shared_ptr <AcceBase> tree;
    if(type == Datastruct::KDTREE)
        tree = KdTree::buildfromnode(treedata, dimension);
    else
        tree = Tsvq::buildfromnode(treedata);
    double * resultpixel = new double [3 * row * col * coe * coe];
    for(int i = 0; i < row * coe; ++i)
        for(int j = 0; j < col * coe; ++j){
            fprintf(stderr,"\rgenerating: %5.2f%%",100. * (i * col * coe + j) / (row * col * coe * coe));
            if(((i - offset) < 0) || ((j - offset) < 0)||(j + offset >= col * coe)){
                resultpixel[3 * (i * (col * coe) + j) + 0] = samplepixel[3 * (((i % row) * col) + (j % col)) + 0];
                resultpixel[3 * (i * (col * coe) + j) + 1] = samplepixel[3 * (((i % row) * col) + (j % col)) + 1];
                resultpixel[3 * (i * (col * coe) + j) + 2] = samplepixel[3 * (((i % row) * col) + (j % col)) + 2];
            }
            else{
                vector <double> queryvt;
                for(int k1 = -offset; k1 <= offset; ++k1){
                    for(int k2 = -offset; k2 <= offset; ++ k2){
                        if(k1 == 0 && k2 == 0)
                            break;
                        int x = i + k1;
                        int y = j + k2;
                        queryvt.push_back(resultpixel[3 * (x * col * coe + y) + 0]);
                        queryvt.push_back(resultpixel[3 * (x * col * coe + y) + 1]);
                        queryvt.push_back(resultpixel[3 * (x * col * coe + y) + 2]);
                    }
                    if(k1 == 0)
                        break;
                }
                auto res = tree->query(queryvt);
                resultpixel[3 * (i * (col * coe) + j) + 0] = samplepixel[3 * res.second + 0];
                resultpixel[3 * (i * (col * coe) + j) + 1] = samplepixel[3 * res.second + 1];
                resultpixel[3 * (i * (col * coe) + j) + 2] = samplepixel[3 * res.second + 2];
            }
        }
    Mat M(row * coe, col * coe, CV_8UC3, Scalar(0,0,255));//generate Mat
    {
        int index = 0;
        for(auto i = M.begin<Vec3b>(); i != M.end<Vec3b>(); ++i){
            (*i)[0] = resultpixel[index++];
            (*i)[1] = resultpixel[index++];
            (*i)[2] = resultpixel[index++];
        }
    }
    imwrite(str + "_out.jpg", M);
    return sample;
}
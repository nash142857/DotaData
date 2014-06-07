//
//  TextureSynthesis.h
//  TextureSynthesis
//
//  Created by slgu on 4/28/14.
//  Copyright (c) 2014 gsl. All rights reserved.
//

#ifndef __TextureSynthesis__TextureSynthesis__
#define __TextureSynthesis__TextureSynthesis__

#include <iostream>
#include "KdTree.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;
enum class Datastruct{
    KDTREE, TSVQ
};
namespace  TextureSynthesis{
    Mat change(const string & str, int coe, int neighborsize, Datastruct type);
}
#endif /* defined(__TextureSynthesis__TextureSynthesis__) */

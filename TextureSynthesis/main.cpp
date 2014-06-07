//
//  main.cpp
//  TextureSynthesis
//
//  Created by slgu on 4/28/14.
//  Copyright (c) 2014 gsl. All rights reserved.
//
#include "KdTree.h"
#include "AcceBase.h"
#include "Tsvq.h"
#include "TextureSynthesis.h"
#include "Graphcut.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;
const int N = 500;
const int Q = 500;
const string basedir = "/Users/gsl/Desktop/";
void test(){
    vector < pair < vector <double> , int> > test;
    time_t st, ed;
    vector <vector <double> > res1;
    vector <vector <double> > res2;
    vector <vector <double> > to_be_query;
    st = clock();
    for(int i = 0; i < N; ++i){
        test.push_back(make_pair(vector<double>({rand() * 100. / RAND_MAX, rand() * 10000. / RAND_MAX}), 2));
    }
    for(int i = 0; i < Q; ++i){
        to_be_query.push_back({rand() * 100. / RAND_MAX, rand() * 10000. / RAND_MAX});
    }
    shared_ptr <AcceBase> init = Tsvq::buildfromnode(test);
    for(int i = 0; i < Q; ++i){
        printf("tsvq: %d\n", i);
        res1.push_back(init -> query(to_be_query[i]).first);
    }
    ed = clock();
    cout << "using tsvqquery" << 1.0 * (ed - st) / CLOCKS_PER_SEC << endl;
    st = clock();
     
    init = KdTree::buildfromnode(test, test.size());
    for(int i = 0; i < Q; ++i){
        printf("kd: %d\n", i);
        res2.push_back(init -> query(to_be_query[i]).first);
    }
    ed = clock();
    cout << "using kdquery" << 1.0 * (ed - st) / CLOCKS_PER_SEC << endl;
    int same = 0;
    for(int i = 0; i < res1.size(); ++ i){
        if(res1[i][0] != res2[i][0] || res1[i][1] != res2[i][1]);
        else
            ++same;
    }
    std::cout << 1.0 * same / res1.size() << std::endl;
}
int main(int argc, const char * argv[])
{
 //   test();
    
    int type = 0;
    int neighbor = 0;
    int codewords = 0;
    puts("using how many neighbor");
    scanf("%d", &neighbor);
    puts("using kdtree or tsvq 0 for kdtree and 1 for tsvq");
    scanf("%d", &type);
    if(type == 0)
        TextureSynthesis::change(basedir + "test1", 2, neighbor, Datastruct::KDTREE);
    else
        TextureSynthesis::change(basedir + "test1", 2, neighbor, Datastruct::TSVQ);
     
   // Graphcut::graphchange(basedir + "test2", 5);
    return 0;
}


//
//  Graphcut.cpp
//  TextureSynthesis
//
//  Created by slgu on 5/12/14.
//  Copyright (c) 2014 gsl. All rights reserved.
//

#include "Graphcut.h"
Dinic graphcut;
void Graphcut::graphchange(const string & filename, int coe){
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    const int blend = 10;
    int diff[300][100];//graph energy difference
    Mat sample;
    sample = imread(filename + ".jpg");
    int row = sample.rows;
    int col = sample.cols;
    int * cutpoint = new int[blend * (col + row)];
    int * samplepixel = new int[3 * row * col];
    int newrow = row * coe - blend * (coe - 1);
    int newcol = col * coe - blend * (coe - 1);
    printf("%d %d\n", newrow, newcol);
    double * resultpixel = new double[3 * newrow * newcol];
    {
        int index = 0;
        for(auto i = sample.begin<Vec3b>(); i != sample.end<Vec3b>(); ++i){
            samplepixel[index++] = (*i)[0];
            samplepixel[index++] = (*i)[1];
            samplepixel[index++] = (*i)[2];
        }
    }
    //init
    for(int i = 0; i < row; ++i)
        for(int j = 0; j < col; ++j){
            resultpixel[3 * (i * newcol + j)] = samplepixel[3 * (i * col + j)];
            resultpixel[3 * (i * newcol + j) + 1] = samplepixel[3 * (i * col + j) + 1];
            resultpixel[3 * (i * newcol + j) + 2] = samplepixel[3 * (i * col + j) + 2];
        }
    for(int i = 0; i < coe; ++i){
        for(int j = 0; j < coe; ++j){
            if(i == 0 && j == 0)
                continue;
            int lefx =  i * (row - blend);
            int lefy =  j * (col - blend);
            int num = 0;
            vector < pair<int, int> > vt;
            map < pair <int, int>, int> mp;
            int s, t;
            if(i == 0){
                s = row * blend;
                t = s + 1;
                graphcut.init(row * blend, row * blend + 1);
                //init the s t the max 1e9
                for(int k = 0; k < row; ++k){
                    graphcut.add_edge(s, k * blend, 1e9);
                    graphcut.add_edge(k * blend + blend - 1, t, 1e9);
                }
                
                //init diff
                for(int o1 = 0; o1 < row; ++o1)
                    for(int o2 = 0; o2 < blend; ++o2){
                        vt.push_back(make_pair(o1, o2));
                        mp[make_pair(o1, o2)] = num++;
                        int px = lefx + o1;
                        int py = lefy + o2;
                        int tmp = 0;
                        for(int dimension = 0; dimension < 3; ++ dimension)
                            tmp += (resultpixel[3 * (px * newcol + py) + dimension] - samplepixel[3 * (o1 * col + o2) + dimension]) * (resultpixel[3 * (px * newcol + py) + dimension] - samplepixel[3 * (o1 * col + o2) + dimension]);
                        diff[o1][o2] = static_cast<int>(sqrt(tmp));
                    }
                
                
                //add edge
                for(int i = 0; i < num; ++i){
                    int o1 = vt[i].first;
                    int o2 = vt[i].second;
                    for(int dic = 0; dic < 4; ++dic){
                        int x = o1 + dx[dic];
                        int y = o2 + dy[dic];
                        if(mp.find(make_pair(x, y)) == mp.end())
                            continue;
                        int rev_i = mp[make_pair(x, y)];
                        graphcut.add_edge(i, rev_i, diff[o1][o2] + diff[x][y]);
                    }
                }
            }
            else if(j == 0){
                s  = col * blend;
                t = s + 1;
                graphcut.init(s, t);
                //init the s t the max 1e9
                for(int k = 0; k < col; ++k){
                    graphcut.add_edge(s,  k, 1e9);
                    graphcut.add_edge(col * blend - k - 1, t, 1e9);
                }
                
                //init diff
                for(int o1 = 0; o1 < blend; ++o1)
                    for(int o2 = 0; o2 < col; ++o2){
                        vt.push_back(make_pair(o1, o2));
                        mp[make_pair(o1, o2)] = num++;
                        int px = lefx + o1;
                        int py = lefy + o2;
                        int tmp = 0;
                        for(int dimension = 0; dimension < 3; ++ dimension)
                            tmp += (resultpixel[3 * (px * newcol + py) + dimension] - samplepixel[3 * (o1 * col + o2) + dimension]) * (resultpixel[3 * (px * newcol + py) + dimension] - samplepixel[3 * (o1 * col + o2) + dimension]);
                        diff[o1][o2] = static_cast<int>(sqrt(tmp));
                    }
                //add edge
                for(int i = 0; i < num; ++i){
                    int o1 = vt[i].first;
                    int o2 = vt[i].second;
                    for(int dic = 0; dic < 4; ++dic){
                        int x = o1 + dx[dic];
                        int y = o2 + dy[dic];
                        if(mp.find(make_pair(x, y)) == mp.end())
                            continue;
                        int rev_i = mp[make_pair(x, y)];
                        graphcut.add_edge(i, rev_i, diff[o1][o2] + diff[x][y]);
                    }
                }
            }
            else{
                s = (row + col) * blend - blend * blend;
                t = s + 1;
                graphcut.init(s, t);
                //note the point -> id
                
                //init diff
                for(int o1 = 0; o1 < blend; ++o1)
                    for(int o2 = 0; o2 < col; ++o2){
                        vt.push_back(make_pair(o1, o2));
                        mp[make_pair(o1, o2)] = num++;
                        int px = lefx + o1;
                        int py = lefy + o2;
                        int tmp = 0;
                        for(int dimension = 0; dimension < 3; ++ dimension)
                            tmp += (resultpixel[3 * (px * newcol + py) + dimension] - samplepixel[3 * (o1 * col + o2) + dimension]) * (resultpixel[3 * (px * newcol + py) + dimension] - samplepixel[3 * (o1 * col + o2) + dimension]);
                        diff[o1][o2] = static_cast<int>(sqrt(tmp));
                    }
                for(int o1 = blend; o1 < row; ++o1)
                    for(int o2 = 0; o2 < blend; ++o2){
                        vt.push_back(make_pair(o1, o2));
                        mp[make_pair(o1, o2)] = num++;
                        int px = lefx + o1;
                        int py = lefy + o2;
                        int tmp = 0;
                        for(int dimension = 0; dimension < 3; ++ dimension)
                            tmp += (resultpixel[3 * (px * newcol + py) + dimension] - samplepixel[3 * (o1 * col + o2) + dimension]) * (resultpixel[3 * (px * newcol + py) + dimension] - samplepixel[3 * (o1 * col + o2) + dimension]);
                        diff[o1][o2] = static_cast<int>(sqrt(tmp));
                    }
                
                 //add edge
                for(int i = 0; i < num; ++i){
                    int o1 = vt[i].first;
                    int o2 = vt[i].second;
                    if(o1 >= blend && o2 == blend - 1)
                        graphcut.add_edge(i, t, 1e9);
                    else
                        if(o1 == blend - 1&& o2 >= blend - 1)
                            graphcut.add_edge(i, t, 1e9);
                    else
                        if(o1 == 0 || o2 == 0)
                            graphcut.add_edge(s, i, 1e9);
                    for(int dic = 0; dic < 4; ++dic){
                        int x = o1 + dx[dic];
                        int y = o2 + dy[dic];
                        if(mp.find(make_pair(x, y)) == mp.end())
                           continue;
                        int rev_i = mp[make_pair(x, y)];
                        graphcut.add_edge(i, rev_i, diff[o1][o2] + diff[x][y]);
                    }
                }
            }
            printf("flow: %d\n", graphcut.flow());
            int cutnum = graphcut.cut(cutpoint);
            //add the data to result
            for(int i = 0; i < cutnum; ++i){
                if(cutpoint[i] == t){
                    continue;
                }
                int o1 = vt[cutpoint[i]].first;
                int o2 = vt[cutpoint[i]].second;
                int newx = o1 + lefx;
                int newy = o2 + lefy;
                for(int dimension = 0; dimension < 3; ++dimension){
                    resultpixel[3 * (newx * newcol + newy) + dimension] = samplepixel[3 * (col * o1 + o2) + dimension];
                }
            }
            for(int o1 = 0; o1 < row; ++o1)
                for(int o2 = 0; o2 < col; ++o2){
                    if(mp.find(make_pair(o1, o2)) == mp.end()){
                        int newx = lefx + o1;
                        int newy = lefy + o2;
                        for(int dimension = 0; dimension < 3; ++dimension){
                            resultpixel[3 * (newx * newcol + newy) + dimension] = samplepixel[3 * (col * o1 + o2) + dimension];
                        }
                    }
                }
        }
    }
    Mat M(newrow, newcol, CV_8UC3, Scalar(0,0,255));//generate Mat
    {
        int index = 0;
        for(auto i = M.begin<Vec3b>(); i != M.end<Vec3b>(); ++i){
            (*i)[0] = resultpixel[index++];
            (*i)[1] = resultpixel[index++];
            (*i)[2] = resultpixel[index++];
        }
    }
    imwrite(filename + "_out.jpg", M);
    delete [] samplepixel;
    delete [] resultpixel;
    delete []cutpoint;
}
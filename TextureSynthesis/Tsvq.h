//
//  Tsvq.h
//  TextureSynthesis
//
//  Created by slgu on 5/5/14.
//  Copyright (c) 2014 gsl. All rights reserved.
//

#ifndef __TextureSynthesis__Tsvq__
#define __TextureSynthesis__Tsvq__
#include "AcceBase.h"
class Tsvq: public AcceBase{
public:
    static shared_ptr <Tsvq> buildfromnode(PointCloud & node){
        shared_ptr <Tsvq> res(new Tsvq());
        res -> root = build(node);
        return res;
    }
    virtual LabelPoint  query(const vector <double> & po){
        LabelPoint res;
        innerquery(root, po, res);
        printf("%d\n", res.second);
        return res;
    }
    virtual ~Tsvq(){
        
    }
private:
    int childnum;
    struct TsvqNode{
        vector <double> center;
        int label;
        TsvqNode(){
            left = right = nullptr;
        }
        shared_ptr <TsvqNode> left;
        shared_ptr <TsvqNode> right;
    };
    shared_ptr <TsvqNode> root;
    Tsvq(){
        
    }
    void innerquery(shared_ptr <TsvqNode> node, const vector <double> & po, LabelPoint & res){
        if(node -> label != -1){
            res = make_pair(node -> center, node -> label);
            return;
        }
        int index = -1;
        double maxn = -1;
        double dis1 = dis(node -> left -> center, po);
        double dis2 = dis(node -> right -> center, po);
        if(dis1  < dis2)
            innerquery(node -> left, po, res);
        else
            innerquery(node -> right, po, res);
    }
    static shared_ptr <TsvqNode> build(const PointCloud & node){
        if(node.size() == 1){
            shared_ptr <TsvqNode> res(new TsvqNode());
            res -> center = node[0].first;
            res -> label = node[0].second;
            return res;
        }
        shared_ptr <TsvqNode> res(new TsvqNode());
        res -> label = -1;//stand for not a leaf.
        vector <double> center(node.size(), 0);
        for(int i = 0; i < node.size(); ++i){
            add(center, node[i].first);
        }
        div(center, node.size());
        res -> center = center;
        PointCloud left;
        PointCloud right;
        kmeans(node, left, right);
        res -> left = build(left);
        res -> right = build(right);
        return res;
    }
    static void kmeans(const PointCloud & node, PointCloud & left, PointCloud & right){
        vector <bool> flag(node.size());
        int dimension = node[0].first.size();
        int n = node.size();
        vector <double> center1(n, 0);
        vector <double> center2(n, 0);
        //init the center
        double maxn = -1;
        int l, r;
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j){
                double dist = dis(node[i].first, node[j].first);
                if(maxn == -1 || maxn < dist){
                    l = i;
                    r = j;
                    maxn = dist;
                }
            }
        center1 = node[l].first;
        center2 = node[r].first;
        double cost = 1e9;
        while(true){
            double nxcost = 0;
            for(int i = 0; i < n; ++i){
                double dis1 = dis(node[i].first, center1);
                double dis2 = dis(node[i].first, center2);
                if(dis1 < dis2){
                    flag[i] = true;
                    nxcost += dis1;
                }
                else{
                    flag[i] = false;
                    nxcost += dis2;
                }
            }
            if(fabs(nxcost - cost) < 1e-8){
                break;
            }
            int num1 = 0, num2 = 0;
            for(int i = 0; i < dimension; ++i)
                center1[i] = center2[i] = 0;
            for(int i = 0; i < n; ++i){
                if(flag[i]){
                    ++num1;
                    add(center1, node[i].first);
                }
                else{
                    ++num2;
                    add(center2, node[i].first);
                }
            }
            div(center1, num1);
            div(center2, num2);
            cost = nxcost;
        }
        left.clear();
        right.clear();
        for(int i = 0; i < n; ++i){
            if(flag[i]){
                left.push_back(node[i]);
            }
            else
                right.push_back(node[i]);
        }
    }
};
#endif /* defined(__TextureSynthesis__Tsvq__) */

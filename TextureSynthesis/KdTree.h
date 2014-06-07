//
//  KdTree.h
//  TextureSynthesis
//
//  Created by slgu on 4/28/14.
//  Copyright (c) 2014 gsl. All rights reserved.
//

#ifndef __TextureSynthesis__KdTree__
#define __TextureSynthesis__KdTree__
#include "AcceBase.h"
using namespace std;
class KdTree: public AcceBase{
public:
    static shared_ptr <KdTree> buildfromnode(vector < pair <vector <double>, int> > & node, int n){
        // n represents the dimension
        shared_ptr <KdTree> res(new KdTree());
        res -> root = buildnode(node, n, 0, int(node.size()) - 1);
        return res;
    }
    virtual pair < vector <double> , int >  query(const vector <double> & po){
        cnt = 0;
        pair <double, shared_ptr < KdTreeNode > > res = make_pair(1e9, nullptr);
        innerquery(po, root, res);
      //  printf("nodenum: %d", cnt);
        return make_pair(res.second -> vt, res.second -> id);
    }
    vector <double> badsearch(const vector <double> & po){
        queue <shared_ptr <KdTreeNode> > q;
        q.push(root);
        pair <double, shared_ptr < KdTreeNode > > res = make_pair(1e9, nullptr);
        while(!q.empty()){
            auto tmp = q.front();
            q.pop();
            double dist = dis(tmp -> vt, po);
            if(dist < res.first){
                res.first = dist;
                res.second = tmp;
            }
            if(tmp -> left != nullptr)
                q.push(tmp -> left);
            if(tmp -> right != nullptr)
                q.push(tmp -> right);
        }
        return res.second -> vt;
    }
    virtual ~KdTree(){
        
    }
private:
    struct KdTreeNode{
        int id;
        int split;
        vector <double> vt;
        shared_ptr <KdTreeNode> left;
        shared_ptr <KdTreeNode> right;
    };
    int cnt;
    shared_ptr <KdTreeNode> root;
    KdTree(){
    }
    void innerquery(const vector <double> & po, shared_ptr <KdTreeNode> node,
                                        pair <double, shared_ptr<KdTreeNode> > & res){
        
        if(node == nullptr)
            return;
        ++cnt;
        double mindis = dis(po, node -> vt);
        auto tmp = make_pair(mindis, node);
        if(tmp.first != -1 && tmp.first < res.first){
            res = tmp;
        }
        if(po[node -> split] <= node -> vt[node -> split]){
            innerquery(po, node -> left, res);
            if(res.first > fabs(po[node -> split] - node -> vt[node -> split])){
                innerquery( po, node -> right, res);//query another side
            }
        }
        else{
            innerquery(po, node -> right, res);
            if(res.first > fabs(po[node -> split] - node -> vt[node -> split])){
                innerquery(po, node -> left, res);
            }
        }
    }
    static shared_ptr <KdTreeNode> buildnode(vector < pair <vector <double>, int> > & node, int n, int l, int r){
        if(l > r)
            return nullptr;//leaf
        double maxn = 0, index = -1;
        for(int i = 0; i < n; ++i) {// cal the max variance dimension
            double tmp = 0, aver = 0;
            for(int j = l; j <= r; ++j){
                aver += node[j].first[i];
            }
            aver /= node.size();
            for(int j = l; j <= r; ++j){
                tmp += (aver - node[j].first[i]) * (aver - node[j].first[i]);
            }
            if(maxn < tmp){
                maxn = tmp;
                index = i;
            }
        }
        sort(node.begin() + l, node.begin() + r + 1,
        [&] (const pair <vector <double>, int>  & a, const pair <vector <double>, int> &b) -> bool
        {
            return a.first[index] < b.first[index];
        }
        );
        shared_ptr <KdTreeNode> res(new KdTreeNode());
        res -> split = index;
        int middle = (l + r) >> 1;//split node
        res -> split = index;
        res -> vt = node[middle].first;
        res -> left = buildnode(node, n, l, middle - 1);
        res -> right = buildnode(node, n,  middle + 1, r);
        res -> id = node[middle].second;
        return res;
        
    }
};

#endif /* defined(__TextureSynthesis__KdTree__) */

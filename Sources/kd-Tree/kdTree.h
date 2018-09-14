#ifndef KDTREE_H
#define KDTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <iomanip>
#include <queue>

#include <gmtl/gmtl.h>

#define DIM 3

using namespace std;

class kdTree{
public:
	kdTree();
	~kdTree();

	void buildkdTree(vector<gmtl::Point3f> &object_Position, int axis);
	vector<int> getRangeValues(float depth, int axis);

private:
	struct kd_Node{
		int obj_number;
		gmtl::Point3f node;

		struct kd_Node *left, *right;
	};

	kd_Node *tree, *root;
	bool _is_root;

	vector<gmtl::Point3f> sorted_x;
	vector<gmtl::Point3f> sorted_y;
	vector<gmtl::Point3f> sorted_z;
	
	vector<gmtl::Point3f> values;
	vector<int> aabd;
	
	void init();
	//kdTree::kd_Node* buildTree(kd_Node *tree, int start, int size, int axis);
	kd_Node* buildTree(kd_Node *tree, int size, int dim, int axis);
	
	void swap(kd_Node *x, kd_Node *y);
	void setTreeRoot(kd_Node *node);
	
	kdTree::kd_Node* getRoot();

	kdTree::kd_Node* getMedian(kd_Node* start, kd_Node* end, int axis);

	vector<gmtl::Point3f> getRangeTuples(kd_Node* root, float d1, float d2, int axis);
	
	vector<gmtl::Point3f> sortObjects(vector<gmtl::Point3f> &a, int first, int last, int axis);
	kdTree::kd_Node* pivot(vector<gmtl::Point3f> &a, int first, int last, int axis);
	void swap(gmtl::Point3f& a, gmtl::Point3f& b);

	void print();
};
#endif

#include "kdTree.h"
#include <algorithm>

kdTree::kdTree(){
}

kdTree::~kdTree(){
}

void kdTree::init(){
}

void kdTree::buildkdTree(vector<gmtl :: Point3f> &object_Position, int axis){
	init();

	tree = (kd_Node *) calloc(object_Position.size(), sizeof(kd_Node));

	for(int i = 0; i < object_Position.size(); i++){
		tree[i].obj_number = i;
		tree[i].node = object_Position.at(i);
	}

	root = buildTree(tree, object_Position.size(), DIM, axis);

	std::cout << "ROOT: " << root->node << std::endl;

	setTreeRoot(root);

//	print();
}

kdTree::kd_Node* kdTree::buildTree(kdTree::kd_Node *tree, int size, int dim, int axis){
	kd_Node *median;

	if(size == 0)
		root = NULL;
	if(size == 1)
		root = tree;

	if((median  = getMedian(tree, tree + size, axis))){
		median->left = buildTree(tree, median - tree, dim, axis);
		median->right = buildTree(median + 1, tree + size - (median + 1), dim, axis);
	}
	return median;
}

kdTree::kd_Node* kdTree::getMedian(kdTree::kd_Node *start, kdTree::kd_Node *end, int axis){
	if (end <= start)
		return NULL;
	if (end == start + 1)
		return start;

	kd_Node *p, *store, *md = start + (end - start) / 2;

	double pivot;

	while (1) {
		pivot = md->node[axis];
		
		swap(md, end - 1);

		store = start;

		for (p = start; p < end - 1; p++) {
			if (p->node[axis] < pivot) {
				if (p != store)
					swap(p, store);
				store++;
			}
		}
		swap(store, end - 1);

		/* median has duplicate values */
		if (store->node[axis] == md->node[axis]){
			return md;
		}

		if (store > md)	
			end = store;
		else
			start = store;
	}
}

void kdTree::swap(kdTree::kd_Node *x, kdTree::kd_Node *y) {
	gmtl::Point3f temp;
	int obj_Num;

	temp = x->node;
	obj_Num = x->obj_number;

	x->node = y->node;
	x->obj_number = y->obj_number;

	y->node = temp;
	y->obj_number = obj_Num;
}

void kdTree::setTreeRoot(kd_Node *node){
	root = node;
}

kdTree::kd_Node* kdTree::getRoot(){
	return root;
}

vector<int> kdTree::getRangeValues(float depth, int axis){
	float d1 = depth - 0.100, d2 = depth + 0.100;

	const clock_t begin_time = clock();
	
	values.clear();
	aabd.clear();

	vector<gmtl::Point3f> te = getRangeTuples(getRoot(), d1, d2, axis);

	return aabd;
}

vector<gmtl::Point3f> kdTree::getRangeTuples(kdTree::kd_Node* root, float d1, float d2, int axis){		
	if(root == NULL){
		return values;
	}
	if(d1 < root->node[axis] && d2 >= root->node[axis]){
		aabd.push_back(root->obj_number);
		values.push_back(root->node);
	}
	if(d1 < root->node[axis]){
		getRangeTuples(root->left, d1, d2, axis);
	}
	if(d2 >= root->node[axis])
		getRangeTuples(root->right, d1, d2, axis);

	return values;
}

void kdTree::print() {
	if (!root) 
		return;

	queue<kd_Node*> nodesQueue;

	int nodesInCurrentLevel = 1;
	int nodesInNextLevel = 0;

	nodesQueue.push(root);
	
	while (!nodesQueue.empty()) {
		kd_Node *currNode = nodesQueue.front();
		nodesQueue.pop();
		nodesInCurrentLevel--;
		
		if (currNode){
			cout << currNode->node<< " ";
			if(currNode->left)
				cout << "1: " << currNode->obj_number <<"-" << currNode->left->node;
			if(currNode->right)
				 cout << ", 2: " << currNode->obj_number <<"-" << currNode->right->node << endl;
			if(currNode->left){
				nodesQueue.push(currNode->left);
				nodesInNextLevel++;
			}
			if(currNode->right){
				nodesQueue.push(currNode->right);
				nodesInNextLevel++;
			}
		}
		if (nodesInCurrentLevel == 0) {
			cout << endl;
			nodesInCurrentLevel = nodesInNextLevel;
			nodesInNextLevel = 0;
		}
	}
}
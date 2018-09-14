#include "KDTree_Oper.h"

static kdtree2::KDTree* tree;

KDTree_Oper::KDTree_Oper(){
}

KDTree_Oper::~KDTree_Oper(){
}

void KDTree_Oper::setObjectXYZ(std::vector<gmtl::Point3f> object_XYZ){
	_kdTree_Nodes_XYZ = object_XYZ;
}

void KDTree_Oper::BuildKDTree(){
	array2dfloat data(boost::extents[_kdTree_Nodes_XYZ.size()][3]);  // declare a 10000 x 3 array.
	array2dfloat realdata; 

	// notice it is in C-standard layout. 
	int N = _kdTree_Nodes_XYZ.size(), dim = 3;

	realdata.resize(boost::extents[N][dim]); 
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < dim; j++)
			realdata[i][j] = _kdTree_Nodes_XYZ[i][j]; // random_variate();
	}

	tree = new kdtree2::KDTree(realdata,true);
	tree->sort_results = true;
	std::cout << "Tree created!!!"; 

	//N_NearestNeighborSearch(6);
}

void KDTree_Oper::N_NearestNeighborSearch(std::vector<float>& qv, int highlight_id, int n, kdtree2::KDTreeResultVector& result){
	tree->sort_results = true;

	time_a_search(tree, qv, highlight_id, n, 50, result); 
	//time_random_searches(tree, n); 
}

void KDTree_Oper::time_a_search(kdtree2::KDTree* tree, std::vector<float>& qv, int highlight_id, int nn, int nsearch, kdtree2::KDTreeResultVector& result){
	int dim = tree->dim;
	
	std::vector<float> query(dim);
	for (int i = 0; i < nsearch;i++)
		tree->n_nearest(qv, nn, result);
}

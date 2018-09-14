#ifndef KDTREE_OPER_H
#define KDTREE_OPER_H

#include "kdtree2.h"

class KDTree_Oper{
public:
	KDTree_Oper();
	~KDTree_Oper();

	void setObjectXYZ(std::vector<gmtl::Point3f> object_XYZ);
	void BuildKDTree();
	
	void N_NearestNeighborSearch(std::vector<float>& qv, int highlight_id, int n, kdtree2::KDTreeResultVector& result);
	std::vector<int>& getNearestNeighbors();
private:
	std::vector<gmtl::Point3f> _kdTree_Nodes_XYZ;

	typedef boost::multi_array<float,2> array2dfloat;

	kdtree2::KDTreeResultVector res; 

	void time_a_search(kdtree2::KDTree* tree, std::vector<float>& qv, int highlight_id, int nn, int nsearch, kdtree2::KDTreeResultVector& result);
	void setNearestNeighbors(std::vector<int> nnVec);
};

#endif
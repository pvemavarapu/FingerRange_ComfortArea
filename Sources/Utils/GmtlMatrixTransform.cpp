#include "GmtlMatrixTransform"

using namespace osg;

GmtlMatrixTransform::GmtlMatrixTransform() : MatrixTransform()
{
}

GmtlMatrixTransform::GmtlMatrixTransform(const gmtl::Matrix44f& matrix) : MatrixTransform()
{
	setMatrix(matrix);
}

GmtlMatrixTransform::GmtlMatrixTransform(const GmtlMatrixTransform& transform, const CopyOp& copyop)
 : MatrixTransform(transform, copyop)
{    
}

GmtlMatrixTransform::~GmtlMatrixTransform()
{
}

void GmtlMatrixTransform::setMatrix(const gmtl::Matrix44f& matrix)
{
	osg::Matrix osgMatrix(matrix.mData);
	_matrix = osgMatrix; 
	_inverseDirty = true; 
	dirtyBound();
}

const gmtl::Matrix44f GmtlMatrixTransform::getMatrix()
{
	gmtl::Matrix44f matrix;
	
	matrix.set(	_matrix(0, 0), _matrix(1, 0), _matrix(2, 0), _matrix(3, 0), 
				_matrix(0, 1), _matrix(1, 1), _matrix(2, 1), _matrix(3, 1), 
				_matrix(0, 2), _matrix(1, 2), _matrix(2, 2), _matrix(3, 2), 
				_matrix(0, 3), _matrix(1, 3), _matrix(2, 3), _matrix(3, 3)	);
	return matrix;
}
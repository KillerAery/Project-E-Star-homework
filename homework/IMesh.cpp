#include "IMesh.h"

Aery::IMesh::IMesh(int type = 0) :
	m_mesh(nullptr),
	m_meshType(type)
{

}

Aery::IMesh::~IMesh()
{
}

int Aery::IMesh::getType() const
{
	return m_meshType;
}


Mesh* Aery::IMesh::getMesh() const
{
	return m_mesh;
}

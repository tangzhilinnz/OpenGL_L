#include "mesh.h"

Mesh::Mesh(Geometry* geometry, Material* material)
{
    mGeometry = geometry;
    mMaterial = material;
    mType = ObjectType::Mesh;
}

Mesh::Mesh()
{
    mType = ObjectType::Mesh;
}

Mesh::~Mesh()
{
    printf("---- ~Mesh ----\n");
}
#include "mesh.h"


Mesh* Mesh::createObj()
{
    Mesh* mesh = new Mesh;
    bookmark.push_back(mesh);
    return mesh;
}

Mesh* Mesh::createObj(Geometry* geometry, Material* material)
{
    Mesh* mesh = new Mesh(geometry, material);
    bookmark.push_back(mesh);
    return mesh;
}

Mesh::Mesh(Geometry* geometry, Material* material)
    : mState(nullptr),
    mGeometry(geometry),
    mMaterial(material)
{
    mType = ObjectType::Mesh;
}

Mesh::Mesh()
    : mState(nullptr),
    mGeometry(nullptr),
    mMaterial(nullptr)
{
    mType = ObjectType::Mesh;
}

Mesh::~Mesh()
{
    printf("---- ~Mesh ----\n");
}
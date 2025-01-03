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
    : mGeometry(geometry),
    mMaterial(material)
{
    mType = ObjectType::Mesh;
}

Mesh::Mesh()
    : mGeometry(nullptr),
    mMaterial(nullptr)
{
    mType = ObjectType::Mesh;
}

Mesh::~Mesh()
{
    printf("---- ~Mesh ----\n");
}
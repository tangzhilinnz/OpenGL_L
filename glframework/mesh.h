#pragma once

#include"object.h"
#include "geometry.h"
#include "material/material.h"

class Mesh : public Object
{
public:
    // Constructor using smart pointers
    Mesh(Geometry* geometry, Material* material);
    Mesh();
    Mesh(const Mesh&) = default;
    Mesh& operator=(const Mesh&) = default;
    ~Mesh() = default;

    // Getter for mGeometry
    inline Geometry* getGeometry() const
    {
        return mGeometry;
    }

    // Getter for mMaterial
    inline Material* getMaterial() const
    {
        return mMaterial;
    }

    inline void setGeometry(Geometry* geometry)
    {
        mGeometry = geometry;
    }

    inline void setMaterial(Material* material)
    {
        mMaterial = material;
    }

private:
    // Mesh utilizes Geometry and Material without taking responsibility for
    // their memory management or initialization.
    Geometry* mGeometry;
    Material* mMaterial;
};
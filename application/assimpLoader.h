#pragma once

#include "../glframework/core.h"
#include "../glframework/object.h"
#include "../glframework/mesh.h"
#include "../glframework/texture.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class AssimpLoader
{
public:
	static Object* load(const std::string& path);
private:
	static std::string rootPath;
	static const aiScene* scene;

	static void processNode(aiNode* ainode, Object* parent);

	static Mesh* processMesh(aiMesh* aimesh);

	static Texture* processTexture(const aiMaterial* aimat, const aiTextureType& type,
		                           unsigned int unit);

	static glm::mat4 getMat4f(const aiMatrix4x4& mat);
};
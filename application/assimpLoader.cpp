#include"assimpLoader.h"
#include "../glframework/tools/tools.h"
#include "../glframework/material/phongMaterial.h"
#include "../glframework/material/material.h"
#include "../glframework/geometry.h"

#include <stack>

// Define the static members
std::string AssimpLoader::rootPath = "";
const aiScene* AssimpLoader::scene = nullptr;

Object* AssimpLoader::load(const std::string& path)
{
	//拿出模型所在目录
	std::size_t lastIndex = path.find_last_of("//");
	rootPath = path.substr(0, lastIndex + 1);

	Object* rootNode = new Object();

	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

	//验证读取是否正确顺利
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Error:Model Read Failed!" << std::endl;
		return nullptr;
	}

	processNode(scene->mRootNode, rootNode);

	return rootNode;
}

void AssimpLoader::destroy(Object* rootNode)
{
	printf("---- AssimpLoader::destroy ----\n");

	if (!rootNode) return;

	std::stack<Object*> nodeStack;
	nodeStack.push(rootNode);

	// Perform post-order traversal using a stack
	while (!nodeStack.empty())
	{
		Object* cur = nodeStack.top();
		nodeStack.pop();

		// Push all children onto the stack
		for (Object* child : cur->getChildren())
		{
			nodeStack.push(child);
		}

		if(cur->getType() == ObjectType::Mesh)
		{
			auto geometry = ((Mesh*)cur)->getGeometry();
			auto material = ((Mesh*)cur)->getMaterial();

			if (material)
			{
				delete material;
			}

			if (geometry)
			{
				delete geometry;
			}
		}

		delete cur;
	}

	Texture::clearCache();
}

void AssimpLoader::processNode(aiNode* rootNode, Object* parent)
{
	// Use a stack to emulate recursion, storing node-parent pairs
	std::stack<std::pair<aiNode*, Object*>> nodeStack;

	// Push the root node and its parent onto the stack
	nodeStack.push({ rootNode, parent });

	while (!nodeStack.empty())
	{
		// Get the current node and its parent object
		auto nodePair = nodeStack.top();
		auto currentAiNode = nodePair.first;
        auto currentParent = nodePair.second;
		nodeStack.pop();

		// Create a new Object for the current node
		Object* node = new Object();
		currentParent->addChild(node);

		// Process the local transformation
		glm::mat4 localMatrix = getMat4f(currentAiNode->mTransformation);

		// Decompose the transformation matrix into position, rotation, and scale
		glm::vec3 position, eulerAngle, scale;
		Tools::decompose(localMatrix, position, eulerAngle, scale);

		// Set the transformation properties of the node
		node->setPosition(position);
		node->setAngleX(eulerAngle.x);
		node->setAngleY(eulerAngle.y);
		node->setAngleZ(eulerAngle.z);
		node->setScale(scale);

		// Process meshes in the current node
		for (unsigned int i = 0; i < currentAiNode->mNumMeshes; i++)
		{
			int meshID = currentAiNode->mMeshes[i];
			aiMesh* aimesh = scene->mMeshes[meshID];
			auto mesh = processMesh(aimesh);
			node->addChild(mesh);
		}

		// Push all child nodes onto the stack in reverse order to maintain
		// left-to-right processing
		for (int i = currentAiNode->mNumChildren - 1; i >= 0; i--)
		{
			nodeStack.push({ currentAiNode->mChildren[i], node });
		}
	}
}

Mesh* AssimpLoader::processMesh(aiMesh* aimesh) 
{
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
	{
		//第i个顶点的位置
		positions.push_back(aimesh->mVertices[i].x);
		positions.push_back(aimesh->mVertices[i].y);
		positions.push_back(aimesh->mVertices[i].z);

		//第i个顶点的法线
		normals.push_back(aimesh->mNormals[i].x);
		normals.push_back(aimesh->mNormals[i].y);
		normals.push_back(aimesh->mNormals[i].z);

		//第i个顶点的uv
		//关注其第0套uv，一般情况下是贴图uv
		if (aimesh->mTextureCoords[0])
		{
			uvs.push_back(aimesh->mTextureCoords[0][i].x);
			uvs.push_back(aimesh->mTextureCoords[0][i].y);
		}
		else
		{
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
		}
	}

	//解析mesh中的索引值
	for (unsigned int i = 0; i < aimesh->mNumFaces; i++)
	{
		aiFace face = aimesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	auto geometry = new Geometry(positions, normals, uvs, indices);
	auto material = new PhongMaterial();

	if (aimesh->mMaterialIndex >= 0)
	{
		Texture* texture = nullptr;
		aiMaterial* aiMat = scene->mMaterials[aimesh->mMaterialIndex];

		//读取了diffuse贴图
		texture = processTexture(aiMat, aiTextureType_DIFFUSE, 0);
		if (texture == nullptr)
		{
			texture = Texture::createTexture("assets/textures/defaultTexture.jpg", 0);
		}
		texture->setUnit(0);
		material->setDiffuse(texture);

		//读取specular贴图
		auto specularMask = processTexture(aiMat, aiTextureType_SPECULAR, 1);
		if (specularMask == nullptr)
		{
			specularMask = Texture::createTexture("assets/textures/defaultTexture.jpg", 1);
		}
		specularMask->setUnit(1);
		material->setSpecularMask(specularMask);
	}
	else
	{
		material->setDiffuse(Texture::createTexture("assets/textures/defaultTexture.jpg", 0));
		material->setSpecularMask(Texture::createTexture("assets/textures/defaultTexture.jpg", 1));
	}

	return new Mesh(geometry, material);
}

Texture* AssimpLoader::processTexture(const aiMaterial* aimat, const aiTextureType& type,
                                      unsigned int unit)
{
	Texture* texture = nullptr;
	//获取图片读取路径
	aiString aipath;
	aimat->Get(AI_MATKEY_TEXTURE(type, 0), aipath);
	if (!aipath.length)
	{
		return nullptr;
	}

	//判断是否是嵌入fbx的图片
	const aiTexture* aitexture = scene->GetEmbeddedTexture(aipath.C_Str());
	if (aitexture)
	{
		//纹理图片是内嵌的
		unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData);
		uint32_t widthIn = aitexture->mWidth;//通常情况下（png、jpg），代表了整张图片大小
		uint32_t heightIn = aitexture->mHeight;
		texture = Texture::createTextureFromMemory(aipath.C_Str(), unit, dataIn,
			                                       widthIn, heightIn);
	}
	else
	{
		//纹理图片在硬盘上
		std::string fullPath = rootPath + aipath.C_Str();
		texture = Texture::createTexture(fullPath.c_str(), unit);
	}

	return texture;
}

glm::mat4 AssimpLoader::getMat4f(const aiMatrix4x4& mat)
{
	return glm::mat4(
		mat.a1, mat.a2, mat.a3, mat.a4,
		mat.b1, mat.b2, mat.b3, mat.b4,
		mat.c1, mat.c2, mat.c3, mat.c4,
		mat.d1, mat.d2, mat.d3, mat.d4
	);
}
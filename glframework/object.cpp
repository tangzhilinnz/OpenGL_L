#include "object.h"

Object::Object()
{
	mType = ObjectType::Object;
}

void Object::setPosition(glm::vec3 pos)
{
	mPosition = pos;
}

void Object::rotateX(float angle)
{
	mAngleX += angle;
}

void Object::rotateY(float angle)
{
	mAngleY += angle;
}

void Object::rotateZ(float angle)
{
	mAngleZ += angle;
}

void Object::setAngleX(float angle)
{
	mAngleX = angle;
}

void Object::setAngleY(float angle)
{
	mAngleY = angle;
}

void Object::setAngleZ(float angle)
{
	mAngleZ = angle;
}

void Object::setScale(glm::vec3 scale)
{
	mScale = scale;
}

glm::mat4 Object::getModelMatrix() const
{
	glm::mat4 modelMatrix{ 1.0f }; // Initialize with identity matrix
	const Object* current = this;  // Start from the current object

	// Traverse up the hierarchy
	while (current != nullptr)
	{
		glm::mat4 transform{ 1.0f };

		// Apply scaling
		transform = glm::scale(transform, current->mScale);

		// Apply rotations (pitch, yaw, roll)
		transform = glm::rotate(transform, glm::radians(current->mAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(current->mAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(current->mAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

		// Apply translation
		transform = glm::translate(glm::mat4(1.0f), current->mPosition) * transform;

		// Accumulate transformations
		modelMatrix = transform * modelMatrix;

		// Move to the parent object
		current = current->mParent;
	}

	return modelMatrix;
}

void Object::addChild(Object* obj)
{
	//1 检查是否曾经加入过这个孩子--返回迭代器
	auto iter = std::find(mChildren.begin(), mChildren.end(), obj);
	if (iter != mChildren.end())
	{
		std::cerr << "Duplicated Child Added" << std::endl;
		return;
	}

	//2 加入孩子
	mChildren.push_back(obj);

	//3 告诉新加入的孩子他的爸爸是谁
	obj->mParent = this;
}
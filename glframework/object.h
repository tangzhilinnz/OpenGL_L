#pragma once

#include "core.h"

#include <vector>
#include <iostream>

enum class ObjectType
{
	Object,
	Mesh,
	Scene
};

class Object
{
public:
	static Object* Object::createObj();

	Object();
	Object(const Object&) = default;
	Object& operator=(const Object&) = default;
	virtual ~Object();

	void setPosition(glm::vec3 pos);

	inline glm::vec3 getPosition() const { return mPosition; }

	//增量旋转
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	//设置旋转角度
	void setAngleX(float angle);
	void setAngleY(float angle);
	void setAngleZ(float angle);

	void setScale(glm::vec3 scale);

	glm::mat4 getModelMatrix() const;

	void addChild(Object* obj);

	const std::vector<Object*>& getChildren() const { return mChildren; }
	Object* getParent() const { return mParent; }
	ObjectType getType()const { return mType; }

	// Uniform method to destroy all instances
	static void destroyAllInstances();
	const std::vector<Object*>& getInstances() { return bookmark; }

protected:
	glm::vec3 mPosition{ 0.0f };

	//unity旋转标准：pitch yaw roll
	float mAngleX{ 0.0f };
	float mAngleY{ 0.0f };
	float mAngleZ{ 0.0f };

	glm::vec3 mScale{ 1.0f };

	//父子关系
	std::vector<Object*> mChildren;
	Object*              mParent{ nullptr };

    ObjectType mType;

protected:
	// Static bookmark to store instances
	static std::vector<Object*> bookmark;
};
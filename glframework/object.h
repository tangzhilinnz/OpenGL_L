#pragma once

#include "core.h"

class Object
{
public:
	Object() = default;
	Object(const Object&) = default;
	Object& operator=(const Object&) = default;
	~Object() = default;

	void setPosition(glm::vec3 pos);

	inline glm::vec3 getPosition() const { return mPosition; }

	//增量旋转
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	void setScale(glm::vec3 scale);

	glm::mat4 getModelMatrix();

protected:
	glm::vec3 mPosition{ 0.0f };

	//unity旋转标准：pitch yaw roll
	float mAngleX{ 0.0f };
	float mAngleY{ 0.0f };
	float mAngleZ{ 0.0f };

	glm::vec3 mScale{ 1.0f };
};
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
	//unity������ ��ת ƽ��
	glm::mat4 transform{ 1.0f };

	transform = glm::scale(transform, mScale);

	//unity��ת��׼��pitch yaw roll
	transform = glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

	transform = glm::translate(glm::mat4(1.0f), mPosition) * transform;

	return transform;
}

void Object::addChild(Object* obj)
{
	//1 ����Ƿ�����������������--���ص�����
	auto iter = std::find(mChildren.begin(), mChildren.end(), obj);
	if (iter != mChildren.end())
	{
		std::cerr << "Duplicated Child Added" << std::endl;
		return;
	}

	//2 ���뺢��
	mChildren.push_back(obj);

	//3 �����¼���ĺ������İְ���˭
	obj->mParent = this;
}
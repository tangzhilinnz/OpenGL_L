#include "tools.h"

void Tools::decompose(
	glm::mat4& matrix,
	glm::vec3& position,
	glm::vec3& eulerAngle,
	glm::vec3& scale)
{
	//��Ԫ����������ʾ��ת�任
	glm::quat quaternion;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(matrix, scale, quaternion, position, skew, perspective);

	//��Ԫ��-ŷ����
	glm::mat4 rotation = glm::toMat4(quaternion);
	glm::extractEulerAngleXYZ(rotation, eulerAngle.x, eulerAngle.y, eulerAngle.z);

	//�⹹�����ĽǶ��ǻ��ȣ�ת��Ϊ�Ƕ�
	eulerAngle.x = glm::degrees(eulerAngle.x);
	eulerAngle.y = glm::degrees(eulerAngle.y);
	eulerAngle.z = glm::degrees(eulerAngle.z);
}
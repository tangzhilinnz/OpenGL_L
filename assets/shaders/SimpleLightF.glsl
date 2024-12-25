#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;

//��Դ����
uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform vec3 ambientLight;

//�������λ��
uniform vec3 cameraPosition;

uniform float specularIntensity;

void main()
{
  //������յ�ͨ������
  vec3 objectColor = texture(sampler, uv).xyz;
  vec3 normalN = normalize(normal);
  vec3 lightDirN = normalize(lightDirection);
  vec3 veiwDir = normalize(worldPosition - cameraPosition);

  //2 ����diffuse
  float diffuse = clamp(dot(-lightDirN, normalN), 0.0f, 1.0f);
  vec3 diffuseColor = lightColor * diffuse * objectColor;

  //����specular
  //��ֹ�����Ч��
  float dotResult = dot(-lightDirN, normalN);
  float flag = step(0.0f, dotResult);

  vec3 lightReflect = normalize(reflect(lightDirN, normalN));

  float specular = max(dot(lightReflect, -veiwDir), 0.0f);

  //���ƹ�ߴ�С
  specular = pow(specular, 64);
  vec3 specularColor = lightColor * specular * flag * specularIntensity;

  //���������
  vec3 ambientColor = objectColor * ambientLight;

  vec3 finalColor = diffuseColor + specularColor + ambientColor;

  FragColor = vec4(finalColor, 1.0f);
}
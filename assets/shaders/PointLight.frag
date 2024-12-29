#version 460 core
out vec4 FragColor;


in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler; //diffuse��ͼ������
uniform sampler2D specularMaskSampler; //specularMask��ͼ������

//��Դ����
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float k2;
uniform float k1;
uniform float kc;

uniform vec3 ambientColor;

//�������λ��
uniform vec3 cameraPosition;

uniform float specularIntensity;

uniform float shiness;

void main()
{
    //������յ�ͨ������
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 normalN = normalize(normal);
    vec3 lightDirN = normalize(worldPosition - lightPosition);
    vec3 viewDir = normalize(worldPosition - cameraPosition);

    //����˥��
    float dist = length(worldPosition - lightPosition);
    float attenuation = 1.0f / (k2 * dist * dist + k1 * dist + kc); 

    //2 ����diffuse
    float diffuse = clamp(dot(-lightDirN, normalN), 0.0f, 1.0f);
    vec3 diffuseColor = lightColor * diffuse * objectColor;
    
    //����specular
	//��ֹ�����Ч��
    float dotResult = dot(-lightDirN, normalN);
    float flag = step(0.0f, dotResult);

    vec3 lightReflect = normalize(reflect(lightDirN, normalN));
    
    float specular = max(dot(lightReflect, -viewDir), 0.0f);
    
    //���ƹ�ߴ�С
    specular = pow(specular, shiness);

    float specularMask = texture(specularMaskSampler, uv).r;

    vec3 specularColor = lightColor * specular * flag * specularIntensity * specularMask;

    //���������
    vec3 ambientColor = objectColor * ambientColor;

    vec3 finalColor = (diffuseColor + specularColor) * attenuation + ambientColor;

    FragColor = vec4(finalColor, 1.0f);
};
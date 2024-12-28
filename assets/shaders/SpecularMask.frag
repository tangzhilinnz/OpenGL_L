#version 460 core
out vec4 FragColor;


in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler; //diffuse��ͼ������
uniform sampler2D specularMaskSampler; //specularMask��ͼ������

//��Դ����
uniform vec3 lightDirection;
uniform vec3 lightColor;

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
    vec3 lightDirN = normalize(lightDirection);
    vec3 viewDir = normalize(worldPosition - cameraPosition);

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

    vec3 finalColor = diffuseColor + specularColor + ambientColor;

    FragColor = vec4(finalColor, 1.0f);
};
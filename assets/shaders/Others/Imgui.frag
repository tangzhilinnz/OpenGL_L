#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler; //diffuse��ͼ������
uniform sampler2D specularMaskSampler; //specularMask��ͼ������
uniform vec3 cameraPosition; //�������λ��
uniform float shiness; //���ϵķ���ϵ��

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float specularIntensity;
};

struct PointLight
{
    vec3 position;
    vec3 color;
    float specularIntensity;

    float k2;
    float k1;
    float kc;
};

struct SpotLight
{
    vec3 position;
    vec3 targetDirection;
    vec3 color;
    float outerLine; //cos��
    float innerLine; //cos��
    float specularIntensity;

    float k2;
    float k1;
    float kc;
};

struct AmbientLight
{
    vec3 color;
};

uniform SpotLight spotLight;
uniform DirectionalLight directionalLight;

//�궨��
#define POINT_LIGHT_NUM 4
uniform PointLight pointLights[POINT_LIGHT_NUM];

uniform AmbientLight ambientLight;

//�������������
vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal)
{
    float diffuse = clamp(dot(-lightDir, normal), 0.0f, 1.0f);
    vec3 diffuseColor = lightColor * diffuse * objectColor;
    return diffuseColor;
}

//���㾵�淴�����
vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity, float specularMask)
{
    //1 ��ֹ�����Ч��
	float dotResult = dot(-lightDir, normal);
	float flag = step(0.0f, dotResult);
	vec3 lightReflect = normalize(reflect(lightDir, normal));

	//2 jisuan specular
	float specular = max(dot(lightReflect, -viewDir), 0.0f);

	//3 ���ƹ�ߴ�С
	specular = pow(specular, shiness);

	//4 ����������ɫ
	vec3 specularColor = lightColor * specular * flag * intensity * specularMask;

	return specularColor;
}

// without specularMask
vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity)
{
    return calculateSpecular(lightColor, lightDir, normal, viewDir, intensity, 1.0f);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	//������յ�ͨ������
	vec3 objectColor  = texture(sampler, uv).xyz;
    float specularMask = texture(specularMaskSampler, uv).r;
	vec3 lightDir = normalize(worldPosition - light.position);
	vec3 targetDir = normalize(light.targetDirection);

    //����˥��
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0f / (light.k2 * dist * dist + light.k1 * dist + light.kc);

	//����spotlight�����䷶Χ
	float cGamma = dot(lightDir, targetDir);
	float rangeIntensity = clamp((cGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0f, 1.0f);

	//����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir,normal);

	//����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity, specularMask); 

	return (diffuseColor + specularColor) * rangeIntensity * attenuation;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal ,vec3 viewDir)
{
	//������յ�ͨ������
	vec3 objectColor  = texture(sampler, uv).xyz;
    float specularMask = texture(specularMaskSampler, uv).r;
	vec3 lightDir = normalize(light.direction);

	//����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir,normal);

	//����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity, specularMask); 

	return diffuseColor + specularColor;
}

vec3 calculatePointLight(PointLight light, vec3 normal ,vec3 viewDir)
{
	//������յ�ͨ������
	vec3 objectColor  = texture(sampler, uv).xyz;
    float specularMask = texture(specularMaskSampler, uv).r;
	vec3 lightDir = normalize(worldPosition - light.position);

	//����˥��
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0f / (light.k2 * dist * dist + light.k1 * dist + light.kc);

	//����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir,normal);

	//����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity, specularMask); 

	return (diffuseColor + specularColor) * attenuation;
}

//���㾵��������
vec3 calculateAmbient(AmbientLight light)
{
     vec3 objectColor  = texture(sampler, uv).xyz;
     return light.color * objectColor;
}

void main()
{
    //������յ�ͨ������
    vec3 normalN = normalize(normal);
    vec3 viewDir = normalize(worldPosition - cameraPosition);

    vec3 result = vec3(0.0,0.0,0.0);

    result += calculateSpotLight(spotLight, normalN, viewDir);
	result += calculateDirectionalLight(directionalLight, normalN, viewDir);

    for(int i = 0;i < POINT_LIGHT_NUM;i++)
    {
		result += calculatePointLight(pointLights[i], normalN, viewDir);
	}

    result += calculateAmbient(ambientLight);

    FragColor = vec4(result, 1.0f);
};
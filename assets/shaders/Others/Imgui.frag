#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler; //diffuse贴图采样器
uniform sampler2D specularMaskSampler; //specularMask贴图采样器
uniform vec3 cameraPosition; //相机世界位置
uniform float shiness; //材料的反光系数

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
    float outerLine; //cosθ
    float innerLine; //cosβ
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

//宏定义
#define POINT_LIGHT_NUM 4
uniform PointLight pointLights[POINT_LIGHT_NUM];

uniform AmbientLight ambientLight;

//计算漫反射光照
vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal)
{
    float diffuse = clamp(dot(-lightDir, normal), 0.0f, 1.0f);
    vec3 diffuseColor = lightColor * diffuse * objectColor;
    return diffuseColor;
}

//计算镜面反射光照
vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity, float specularMask)
{
    //1 防止背面光效果
	float dotResult = dot(-lightDir, normal);
	float flag = step(0.0f, dotResult);
	vec3 lightReflect = normalize(reflect(lightDir, normal));

	//2 jisuan specular
	float specular = max(dot(lightReflect, -viewDir), 0.0f);

	//3 控制光斑大小
	specular = pow(specular, shiness);

	//4 计算最终颜色
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
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
    float specularMask = texture(specularMaskSampler, uv).r;
	vec3 lightDir = normalize(worldPosition - light.position);
	vec3 targetDir = normalize(light.targetDirection);

    //计算衰减
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0f / (light.k2 * dist * dist + light.k1 * dist + light.kc);

	//计算spotlight的照射范围
	float cGamma = dot(lightDir, targetDir);
	float rangeIntensity = clamp((cGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0f, 1.0f);

	//计算diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir,normal);

	//计算specular
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity, specularMask); 

	return (diffuseColor + specularColor) * rangeIntensity * attenuation;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal ,vec3 viewDir)
{
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
    float specularMask = texture(specularMaskSampler, uv).r;
	vec3 lightDir = normalize(light.direction);

	//计算diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir,normal);

	//计算specular
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity, specularMask); 

	return diffuseColor + specularColor;
}

vec3 calculatePointLight(PointLight light, vec3 normal ,vec3 viewDir)
{
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
    float specularMask = texture(specularMaskSampler, uv).r;
	vec3 lightDir = normalize(worldPosition - light.position);

	//计算衰减
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0f / (light.k2 * dist * dist + light.k1 * dist + light.kc);

	//计算diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir,normal);

	//计算specular
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity, specularMask); 

	return (diffuseColor + specularColor) * attenuation;
}

//计算镜环境光照
vec3 calculateAmbient(AmbientLight light)
{
     vec3 objectColor  = texture(sampler, uv).xyz;
     return light.color * objectColor;
}

void main()
{
    //计算光照的通用数据
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
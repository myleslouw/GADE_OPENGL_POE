#version 330
in vec4 vCol;
in vec2 UV;
in vec3 Normals;
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
	vec3 colour;
	float ambient_Intensity;
	float diffuse_Intensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specular_Intensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

//lighting
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

//texture sampler
uniform sampler2D texture1;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction)
{
	vec4 ambientColour = vec4(light.colour,1.0f) * light.ambient_Intensity;

	float diffuseFactor = max(dot(normalize(Normals),normalize(direction)),0.0f);
	vec4 diffuseColour = vec4(light.colour * light.diffuse_Intensity * diffuseFactor, 1.0f);
	vec4 specularColour = vec4(0,0,0,0);

	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition-FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normals)));

		float specularFactor = dot(fragToEye,reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor,material.shininess);
			specularColour = vec4(light.colour * material.specular_Intensity * specularFactor,1.0f);
		}
	}

	return (ambientColour + diffuseColour + specularColour);
}
vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base,directionalLight.direction);
}

vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	vec4 colour = CalcLightByDirection(pLight.base, direction);
	float attenuation = pLight.exponent * distance * distance + pLight.linear * distance + pLight.constant;

	return (colour/attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection,sLight.direction);

	if(slFactor > sLight.edge)
	{
		vec4 colour = CalcPointLight(sLight.base);
		return colour * (1.0f) - (1.0f-slFactor) * (1.0f/(1.0f - sLight.edge));
	}
	else
	{
		return vec4(0,0,0,0);
	}
}
vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0,0,0,0);
	for(int i = 0; i<pointLightCount;i++)
	{
			totalColour += CalcPointLight(pointLights[i]);
	}
	return totalColour;
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0,0,0,0);
	for(int i = 0; i<spotLightCount;i++)
	{
			totalColour += CalcSpotLight(spotLights[i]);
	}
	return totalColour;
}

void main()														
{	
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();

	//colour = vec4(FragPos,1.0) * finalColour;
	//colour = texture(texture1,UV);	
	colour = texture(texture1,UV) * finalColour;
}
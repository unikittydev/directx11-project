/* INCLUDES */

/* STRUCTS */

struct MeshData
{
	float4x4 ltw;
	float4 pos;
	float4 color;
    float4 specular;
	float smoothness;
	float pad[3];
};

struct WorldData
{
	float4x4 _ViewProj;
	float4 _ViewPos;
	float4 _Time;
};

struct MainLight
{
	float4x4 _LightViewProj;
	float4 direction;
	float4 color;
};
    
struct AdditionalLight
{
    // XYZ is a position and Z is a range
	float4 position;
	float4 color;
};

struct AdditionalLights
{
	AdditionalLight lights[8];
	int count;
	float pad[3];
};

struct AmbientLight
{
	float4 color;
};

/* INPUT */

struct VS_IN
{
	float4 pos : POSITION0;
	float4 normal : NORMAL0;
	//float4 tangent : TANGENT0;
	float4 uv : TEXCOORD0;
};

struct PS_IN
{
	float4 posCS : SV_POSITION0;
	float4 normal : NORMAL0;
	//float4 tangent : TANGENT0;
	float4 uv : TEXCOORD0;
	float4 posWS : TEXCOORD1;
};

/* BUFFERS */

cbuffer MESH_DATA : register(b0)
{
	MeshData meshData;
}

cbuffer WORLD_DATA : register(b1)
{
	WorldData worldData;
}

cbuffer MAIN_LIGHT : register(b2)
{
	MainLight mainLight;
}

cbuffer ADDITIONAL_LIGHTS : register(b3)
{
	AdditionalLights additionalLights;
}

cbuffer AMBIENT_LIGHT : register(b4)
{
	AmbientLight ambientLight;
}

/* TEXTURES */

Texture2D _mainTex : register(t0);
SamplerState _mainTex_Sampler : register(s0);

Texture2D _mainShadow : register(t1);
SamplerState _mainShadow_Sampler : register(s1);

/* SHADER */

float3 LightingLambert(in float3 lightColor, in float3 lightDir, in float3 normal)
{
	float NL = saturate(dot(normal, lightDir));
	return lightColor * NL;
}

float3 LightingSpecular(float3 lightColor, in float3 lightDir, in float3 normal, in float3 viewDir, in float3 posWS, in float3 specular, in float smoothness)
{
	float3 refVec = normalize(reflect(lightDir, normal));
	float NH = saturate(dot(viewDir, refVec));
	float modifier = pow(NH, smoothness);
	float3 specReflection = specular * modifier;
	
	return lightColor * specReflection;
}

void GetMainLightData(in float3 normal, out float3 diffuse, out float attenuation)
{
	float3 lightColor = mainLight.color.rgb;
	float3 lightDir = -mainLight.direction.xyz;

	diffuse = LightingLambert(lightColor, lightDir, normal);
	attenuation = 1.0f;
}

void GetMainLightSpecular(in float3 normal, in float3 viewDir, in float3 posWS, in float3 white, in float smoothness, out float3 specular)
{
	float3 lightColor = mainLight.color.rgb;
	float3 lightDir = -mainLight.direction.xyz;

	smoothness = exp2(10 * smoothness + 1);

	specular = LightingSpecular(lightColor, lightDir, normal, viewDir, posWS, white, smoothness);
}

void GetAdditionalLights(in float3 normal, in float3 viewDir, in float3 posWS, in float3 white, in float smoothness,
	out float3 diffuse, out float3 specular, out float attenuation)
{
	float3 _diffuse = 0.0f;
	float3 _specular = 0.0f;
	float _attenuation = 0.0f;

	smoothness = exp2(10 * smoothness + 1);

	int count = additionalLights.count;
	for (int i = 0; i < count; i++)
	{
		AdditionalLight light = additionalLights.lights[i];
		float3 lightPos = light.position.xyz;
		float lightRange = light.position.w;
		float3 lightColor = light.color.rgb;
		float d = distance(lightPos, posWS);

		float shadowAtten = 1.0f;
		float distanceAtten = saturate(1.0f - d * d / lightRange / lightRange);
		float3 lightDir = normalize(lightPos - posWS);

		_diffuse += LightingLambert(lightColor * shadowAtten * distanceAtten, lightDir, normal);
		_specular += LightingSpecular(lightColor * distanceAtten, lightDir, normal, viewDir, posWS, white, smoothness);
		_attenuation += shadowAtten;
	}

	diffuse = _diffuse;
	specular = _specular;
	attenuation = _attenuation;
}

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;
	
	output.posWS = mul(meshData.ltw, float4(input.pos.xyz, 1));
	output.posCS = mul(worldData._ViewProj, output.posWS);
	output.normal = mul(meshData.ltw, input.normal);
	//output.tangent = input.tangent;
	output.uv = input.uv;
	
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 color = _mainTex.Sample(_mainTex_Sampler, input.uv);
	float3 normal = normalize(input.normal.xyz);

	// Main Lights
	float3 diffuseMain = 0;
	float3 specularMain = 0;
	float attenuationMain = 0;

	float3 viewDir = normalize(input.posWS.xyz - worldData._ViewPos.xyz);
	GetMainLightData(normal, diffuseMain, attenuationMain);
	GetMainLightSpecular(normal, viewDir, input.posWS.xyz, meshData.specular.rgb, meshData.smoothness, specularMain);

	float3 ambient = ambientLight.color.rgb;

	// Additional Lights
	float3 diffuseAdditional = 0;
	float3 specularAdditional = 0;
	float attenuationAdditional = 0;

	GetAdditionalLights(normal, viewDir, input.posWS.xyz, meshData.specular.rgb, meshData.smoothness, diffuseAdditional, specularAdditional, attenuationAdditional);

	// Final
	float3 diffuseCombined = diffuseMain + diffuseAdditional;
	float3 specularCombined = specularMain + specularAdditional;
	float attenCombined = attenuationMain + attenuationAdditional;

	float3 finalColor = color.rgb * ((diffuseCombined + specularCombined) * attenCombined + ambient);
	
	return float4(pow(finalColor, 1.0f / 2.2f), 1.0f);
}
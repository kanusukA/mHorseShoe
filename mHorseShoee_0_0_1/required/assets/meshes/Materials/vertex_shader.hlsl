// grass vertex shader
#define sampler2D Sampler2D

struct Sampler2D
{
    Texture2D t;
    SamplerState s;
};

#define SAMPLER2D(name, reg) \
    Texture2D name ## Tex : register(t ## reg);\
    SamplerState name ## State : register(s ## reg);\
    static Sampler2D name = {name ## Tex, name ## State}

float4 tex2D(Sampler2D s, float2 v)
{
    return s.t.SampleLevel(s.s, v, 1.0f);
}

// INPUT
struct vertexIn
{
	float4 pos : POSITION;
	//float4 color : COLOR0;
	float4 texCoord : TEXCOORD0;
	//float4 m03 : TEXCOORD1;
	//float2 mOffset : TEXCOORD2;
    float3 normal : NORMAL;
	
};

// OUTPUT
struct vertexOut
{
	float4 pos : SV_Position;
	float2 texCoord : TEXCOORD0;
};

uniform SAMPLER2D(noiseMap, 0);

vertexOut mainVS(
	vertexIn input,
	uniform float4x4 worldViewProj,
	uniform float4x4 modelView
	) 
	{
		
		vertexOut output = (vertexOut)0;
		float4 disVert = input.pos;
		float4 worldPos = mul(modelView, input.pos);
		/*float3x4 worldMatrix;
		float4 worldPos;
		
		worldMatrix[0] = tex2Dlod( matrixTexture, float4(input.m03.xw + input.mOffset, 0, 0) );
		worldMatrix[1] = tex2Dlod( matrixTexture, float4(input.m03.yw + input.mOffset, 0, 0) );
		worldMatrix[2] = tex2Dlod( matrixTexture, float4(input.m03.zw + input.mOffset, 0, 0) );
		
		
		worldPos = float4( mul( worldMatrix, input.pos).xyz, 1.0f);*/
		//output.color = input.color;
		disVert.x += worldPos.y * input.normal.x;
		disVert.z += worldPos.y * input.normal.z;
		
    float disY = (tex2D(noiseMap, float2(int(disVert.x), int(disVert.z))).r * 0.3) + 0.2;
	
		disVert.y -= (worldPos.y * disY) * (worldPos.y * 0.3);
	
		output.pos =  mul(worldViewProj, disVert);
		//output.pos = worldPos;
		output.texCoord = input.texCoord;
	
	
    return output;
		
	}
	

Texture2D ColorTexture;
SamplerState ColorSampler;

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TextureCoordinates : TEXCOORD;
};

float4 main(VS_OUTPUT IN) : SV_TARGET
{
	return ColorTexture.Sample(ColorSampler, IN.TextureCoordinates);
}
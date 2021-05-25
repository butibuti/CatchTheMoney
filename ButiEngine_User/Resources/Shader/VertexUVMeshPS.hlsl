#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv));
	return float4(Tex.rgb, Tex.a*lightDir.w);
}

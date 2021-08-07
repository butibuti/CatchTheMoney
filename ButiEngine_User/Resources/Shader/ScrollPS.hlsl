#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV_Normal pixel) : SV_TARGET
{
	float2 uv;
	uv.x =frac( pixel.uv.x+ ExInfo.w);
	uv.y= pixel.uv.y;

	float4 Tex = (mainTexture.Sample(mainSampler,uv));
	return Tex;
}

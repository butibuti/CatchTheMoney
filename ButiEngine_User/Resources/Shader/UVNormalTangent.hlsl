#include"DefaultShader.hlsli"

Pixel_UV_Normal_Tangent VSMain(Vertex_UV_Normal_Color vertex)
{
	Pixel_UV_Normal_Tangent output;
	output.position = mul(vertex.position, mvpMatrix);
	output.normal = vertex.normal;
	output.tangent = vertex.color.rgb;
	output.binormal= normalize( (cross(output.tangent, output.normal)* vertex.color.w));
	output.uv = vertex.uv;
	return output;
}
#include"DefaultShader.hlsli"

Vertex_UV_Normal VSMain(Vertex_UV_Normal vertex)
{

	vertex.normal = mul(vertex.normal, (float3x3)modelMatrix);
	return vertex;
}
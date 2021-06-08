#include"DefaultShader.hlsli"

Pixel VSMain(Vertex_UV_Normal_SingleBone vertex)
{
	Pixel output;
	matrix bm = bones[vertex.bone];
	output.position = mul(bm, vertex.position);
	output.position = mul(output.position, mvpMatrix);
	return output;
}
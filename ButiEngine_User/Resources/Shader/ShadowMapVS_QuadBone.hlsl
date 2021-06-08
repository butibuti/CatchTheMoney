#include"DefaultShader.hlsli"

Pixel VSMain(Vertex_UV_Normal_QuadBone vertex)
{
	Pixel output;
	matrix bm = vertex.weight_01 * bones[vertex.boneIndex_1] + vertex.weight_02 * bones[vertex.boneIndex_2] + vertex.weight_03 * bones[vertex.boneIndex_3] + vertex.weight_04 * bones[vertex.boneIndex_4];

	output.position = mul(bm, vertex.position);

	output.position = mul(output.position, mvpMatrix);
	return output;
}
#include"DefaultShader.hlsli"

Pixel VSMain(Vertex vertex)
{
	Pixel output;
	output.position = mul(vertex.position, mvpMatrix);
	return output;
}
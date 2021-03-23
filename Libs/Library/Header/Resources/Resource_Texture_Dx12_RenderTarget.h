#pragma once
#include<d3dx12.h>
#include"Resource_Texture_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;


	class Resource_Texture_Dx12_RenderTarget :public Resource_Texture_Dx12,public IRenderTarget
	{
	public:
		Resource_Texture_Dx12_RenderTarget(int width, int height, std::shared_ptr<GraphicDevice> arg_wkp_graphicDevice);
		void SetRenderTarget(Vector4& arg_clearColor)override;
		void SetRenderTargetWithoutDepth(Vector4& arg_clearColor)override;
		void CreateTextureUploadHeap()override;
		void Initialize()override;
		void DisSetRenderTarget()override;
		void UpdateResourceRelease()override;
		void ResourceUpdate() override;
		void Attach(int slot)override;
		void SetIsCleared(bool arg_isClear)override;
		Vector2 GetSize()override;
	private:

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> renderTargetDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
		//void UpdateResourceRelease()override;
		//void ResourceUpdate() override;
		bool isCleared = false;
	};
}
#pragma once
#include"GraphicDevice.h"
#include <d3d12.h>
namespace ButiEngine {
class DescriptorHeapManager;
class PipelineStateManager;
enum FileFormat
{
	PNG
};
static const unsigned char SamplerTableRegion = 1;
class IApplication;
class GraphicDevice_Dx12 :public GraphicDevice
{
	struct OutputInfo {
		Resource* p_outputResource;
		std::string outputName;
		FileFormat format;
	};
public:

	GraphicDevice_Dx12(std::weak_ptr<IApplication> arg_wkp_application);
	void Initialize()override;

	void PreInitialize()override;

	void Release()override;

	void ClearDepthStancil(const float arg_depth=1.0f) override;

	ID3D12Device& GetDevice();

	ID3D12CommandQueue& GetCommandQueue();

	ID3D12CommandAllocator& GetCommandAllocator();

	ID3D12CommandAllocator& GetCommandAllocator(const UINT arg_index);

	ID3D12CommandAllocator& GetBundleCommandAllocator();

	ID3D12GraphicsCommandList& GetCommandList();

	ID3D12GraphicsCommandList& GetUploadCommandList();

	ID3D12GraphicsCommandList& GetClearCommandList();

	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateSrvSmpCbvMat(const UINT materialCount, const UINT srvCount, const UINT samplerCount, D3D12_ROOT_SIGNATURE_DESC& arg_rootSignatureDesc);

	void SetPipeLine(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	std::pair< Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC> GetRootSignature(const std::wstring& Key);

	std::weak_ptr<DescriptorHeapManager> GetDescriptorHeapManager();

	PipelineStateManager& GetPipelineStateManager();

	void SetRootSignature(const std::wstring& Key, const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootsig, const D3D12_ROOT_SIGNATURE_DESC& arg_desc);

	void AddUploadResource(Resource* rarg_resource);

	void AddOutputResource(Resource* rarg_resource,const FileFormat arg_format,const std::string& arg_fileName);


	void UploadResourceRelease()override; 
	void UploadResourceBufferMerge()override;

	void UnSetCommandList();

	ID3D12Fence& GetFence();

	IDXGISwapChain& GetSwapChain();

	void WaitGPU();

	void Present() override;

	void DrawEnd();
	void StartGUICommand();
	void EndGUICommand();

	void ClearWindow() override;

	void SetCommandList(ID3D12GraphicsCommandList* arg_currentCommandList,const int index=0);
	void SetDefaultRenderTarget()override;
	void SetDefaultDepthStencil();
	void CommandList_SetScissorRect();

	void CommandList_SetRenderTargetView();

	void PushRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& arg_rtvHandle);

	void InsertCommandList();

	void ResourceUpload()override;

	void DrawStart()override;
	void PipelineClear()override;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDsvHeap() const;

	ID3D12Resource& GetTextureUploadHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() const;

	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;

	const D3D12_RECT& GetScissorRect()const;

	UINT GetFrameCount() {
		return FrameCount;
	}

	UINT GetFrameIndex() {
		return frameIndex;
	}

	void SetDepthStencil(D3D12_CPU_DESCRIPTOR_HANDLE*  arg_dsv);
	D3D12_CPU_DESCRIPTOR_HANDLE* GetDepthStencil();
	void DisSetDepthStencil();
	void ResetPipeLine()override;
private:
	D3D12_CPU_DESCRIPTOR_HANDLE* currentDSV = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE defaultDSVHandle;
	static const UINT FrameCount = 3;
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator[FrameCount];

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> bundleCommandAllocator;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> clearCommandList;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> presentCommandList;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> uploadCommandList;

	std::vector<Resource*> vec_uploadResources;
	std::vector<Resource*> vec_uploadResourcesBuffer;
	std::vector<Resource*> vec_befUploadResources;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> drawCommandList;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> guiCommandList;

	std::shared_ptr<DescriptorHeapManager> shp_descripterManager;

	std::shared_ptr<PipelineStateManager> shp_pipelineStateManager;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> vec_renderTargetHandles;

	ID3D12GraphicsCommandList* p_currentCommandList; 
	ID3D12PipelineState* currentPipelineState;

	std::vector< ID3D12CommandList*> vec_drawCommandLists;
	Microsoft::WRL::ComPtr<ID3D12Fence>  fence;

	std::map<std::wstring, std::pair<Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC>> map_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[FrameCount];

	Microsoft::WRL::ComPtr<ID3D12Resource> resourceUploadHeap;
	D3D12_RECT scissorRect;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencil;
	UINT depthDescriptorSize, renderTargetDescriptorSize;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>  renderTargetDescripterHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthStencilDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE ary_rtvHandle[FrameCount];


	Microsoft::WRL::ComPtr<ID3D12RootSignature>  clearRootSignature;


	Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain3;
	UINT frameIndex;
	HANDLE  fenceEvent;
	UINT64  fenceValue;

	std::vector<OutputInfo > vec_outputInfo;
};
}
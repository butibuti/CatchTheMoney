#pragma once
#include"stdafx.h"
#include"IDrawData.h"
#include"../../Common/CBuffer_Dx12.h"
namespace ButiEngine {
	struct DrawData_Dx12;

	using MatrixUpdateFunc = void (DrawData_Dx12::*)();

	struct DrawData_Dx12 :public MeshDrawData {
		void Initialize(const UINT srvCount);

		void CreatePipeLineState(const UINT arg_exCBuffer,const UINT srvCount);
		void CommandExecute();
		void BufferUpdate()override;
		void SetInfo();
		void SetInfo_WithoutCommand();
		void CommandSet();
		std::shared_ptr<ICBuffer> AddICBuffer(std::shared_ptr<ICBuffer> arg_cbuffer)override;

		bool IsCreated()override;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;

		std::vector< D3D12_GPU_DESCRIPTOR_HANDLE >vec_samplerBufferDescriptorHandle;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeLineDesc;
		//パイプラインステート
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

		D3D12_RASTERIZER_DESC rasterizerStateDesc;
		D3D12_ROOT_SIGNATURE_DESC rootdesc;
		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
		int textureRegion;

		MatrixUpdateFunc p_matrixUpdateFunc;

	private:
		void MatrixUpdater_NoRotation();
		void MatrixUpdater_RemoveDecimalPart();
		void MatrixUpdater_RemoveDecimalPartXY();
		void MatrixUpdater_RemoveDecimalPartYZ();
		void MatrixUpdater_RemoveDecimalPartXZ();
		void MatrixUpdater_default();
		void MatrixUpdater_billBoard();
		void MatrixUpdater_billBoardX();
		void MatrixUpdater_billBoardY();
		void MatrixUpdater_billBoardZ();

	};

	struct MeshDrawData_Dx12 :public DrawData_Dx12 {
		
		MeshDrawData_Dx12(const MeshTag& arg_meshTag, const ShaderTag& arg_shader, const MaterialTag& arg_materialTag, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo,std::shared_ptr<Transform> arg_shp_transform);
		
		MeshDrawData_Dx12(const MeshTag& arg_meshTag,const ShaderTag& arg_shader, const std::vector< MaterialTag>& arg_materialTag, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo, std::shared_ptr<Transform> arg_shp_transform);
		
		MeshDrawData_Dx12(const ModelTag& arg_model, const ShaderTag& arg_shader, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo, std::shared_ptr<Transform> arg_shp_transform);
		
		MeshDrawData_Dx12(const ModelTag& arg_model, const ShaderTag& arg_shader, const std::vector< MaterialTag>& arg_materialTag, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo, std::shared_ptr<Transform> arg_shp_transform);

		void DrawBefore()override;
		void Draw()override;

		void SetTransform(std::shared_ptr<Transform>& arg_transform)override {
			transform = arg_transform->GetMatrix();
		}

		inline float GetZ(const Matrix4x4& arg_vpMatrix)override {
			return  GetMaxZ( arg_vpMatrix);
		}
		void ShowZ() override{

		}

		void Initialize()override;

		void PreInitialize()override {};


		
	};

}

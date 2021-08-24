#include"stdafx.h"
#include"CBuffer_Dx12.h"
namespace ButiEngine {

	class Camera_Dx12 :public Camera
	{
	public:
		Camera_Dx12(const std::string& cameraName, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		Camera_Dx12(const CameraProjProperty& arg_cameraViewProp,const std::string& cameraName, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		~Camera_Dx12();
		void Initialize()override;
		void PreInitialize()override {}
		void Start()override;
		void Stop() const override;
		void SetProjectionTexture(const TextureTag& arg_tag)override;
		void SetDepthStencil(const TextureTag& arg_tag)override;
		void ShowUI()override;
		void End()override;
	private:

		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> drawCommandList;

		D3D12_RECT     scissorRect;
		D3D12_VIEWPORT    viewport;
		std::vector< std::shared_ptr<IRenderTarget>> vec_shp_renderTarget;
		std::shared_ptr<IDepthStencil> shp_depthStencil;
	};
}
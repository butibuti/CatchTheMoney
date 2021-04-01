#pragma once
#include"stdafx.h"
#include"../Resources/DrawData/IDrawObject.h"
namespace ButiEngine {

	namespace Collision {
		template<typename T>
		class CollisionLayer;
	}

	struct RegistCommand {
		UINT* p_index = nullptr;
		bool isAfter;
		bool isShadow;
		std::shared_ptr< IDrawObject> shp_obj = nullptr;
	};

	class IDrawLayer :public IObject{
	public:

		void Initialize()override {};
		void PreInitialize()override{}

		virtual void Clear()=0;
		virtual void BefRendering()=0;
		virtual UINT* Regist(std::shared_ptr< IDrawObject> arg_wkp_drawObject, const bool arg_isAfterRendering, std::shared_ptr<Collision::CollisionPrimitive_Box_OBB> arg_ret_pim = nullptr,const bool arg_isShadow=false)=0;
		virtual void UnRegist(UINT* arg_path, const bool arg_isAfterRendering, const bool arg_isShadow = false)=0;
		virtual void DeleteDrawObj(UINT* arg_path, const bool arg_isAfterRendering)=0;

		virtual void SetShadowCamera(std::shared_ptr<ICamera> arg_shadowCamera)=0;
		virtual void SetShadowTexture(TextureTag arg_textureTag)=0;

		virtual void Rendering()=0;

		virtual void ShadowRendering() = 0;

		virtual std::shared_ptr<ICamera> GetShadowCamera()=0;
		virtual TextureTag GetShadowTexture()=0;
	};

	class IDrawLayer_Shadow  {
	public:

		std::shared_ptr<ICamera> shp_shadowCamera;
		TextureTag shadowTexture;
		bool isShadowDrawed;
	};
	class DrawLayer_Shadow;
	struct DrawLayer :public IDrawLayer{

		inline void ZSort(std::vector < std::shared_ptr< IDrawObject>>& arg_vec_drawObjects) {
			sort(arg_vec_drawObjects.begin(), arg_vec_drawObjects.end(), [](const std::shared_ptr< IDrawObject> pmX, const std::shared_ptr<IDrawObject> pmY) {
				//if(pmX.lock()&&pmY.lock())
				return pmX->GetZ() > pmY->GetZ();
				});
		}
		DrawLayer(std::weak_ptr<IRenderer> arg_wkp_renderer);
		void Initialize()override;
		void Clear()override;
		void BefRendering()override;
		UINT* Regist(std::shared_ptr< IDrawObject> arg_wkp_drawObject, const bool arg_isAfterRendering, std::shared_ptr<Collision::CollisionPrimitive_Box_OBB> arg_ret_pim=nullptr, const bool arg_isShadow = false)override;
		void UnRegist(UINT* arg_path, const bool arg_isAfterRendering, const bool arg_isShadow = false)override;
		void DeleteDrawObj(UINT* arg_path,const bool arg_isAfterRendering)override;

		void SetShadowCamera(std::shared_ptr<ICamera> arg_shadowCamera)override;
		void SetShadowTexture(TextureTag arg_textureTag)override;

		void Rendering()override;
		void ShadowRendering() override;

		std::shared_ptr<ICamera> GetShadowCamera()override;
		TextureTag GetShadowTexture()override;

		std::vector<std::shared_ptr< IDrawObject>> vec_befDrawObj;
		std::vector<std::shared_ptr< IDrawObject>> vec_afterDrawObj;

		std::vector<RegistCommand> vec_registCommandBuff;

		int nowFrameAdditionObjectCount=0;
		int nowFrameAdditionObjectCount_after=0;
		std::vector<UINT*> vec_befIndex;
		std::vector<UINT*> vec_afterIndex;
		std::shared_ptr<Collision::CollisionLayer<IDrawObject>> shp_collisionLayer;
		std::weak_ptr<IRenderer> wkp_renderer;

		std::shared_ptr<DrawLayer_Shadow> shp_shadowLayer;
	};
	class DrawLayer_Shadow : public DrawLayer, public IDrawLayer_Shadow {
	public:
		DrawLayer_Shadow(std::weak_ptr<IRenderer> arg_wkp_renderer):DrawLayer( arg_wkp_renderer){};
		void BefRendering()override;
		void Initialize()override {}
		std::shared_ptr<ICamera> GetShadowCamera()override;
		TextureTag GetShadowTexture()override;
		void SetShadowCamera(std::shared_ptr<ICamera> arg_shadowCamera)override;
		void SetShadowTexture(TextureTag arg_textureTag)override;
	};

	class Renderer:public IRenderer
	{
	public:
		Renderer(std::weak_ptr<IScene> arg_wkp_iscene);
		void Initialize()override;
		void Update()override;
		void RenderingStart()override;
		void BefRendering()override;
		void AddLayer()override;
		UINT GetLayerCount() override;
		void Rendering(const UINT arg_layer )override;
		void ShadowRendering(const UINT arg_layer)override;
		void RenderingEnd()override;
		void ClearDrawObjects()override;
		void Draw(const MeshTag& arg_meshTag)override; 
		std::weak_ptr<IScene> GetScene()override;
		void ShadowTextureAttach(const UINT arg_layer, const UINT arg_slot)override;
		void TextureAttach(const TextureTag& arg_textureTag, const UINT slot)override;
		void ShaderAttach(const ShaderTag& arg_shaderTag)override;
		void MaterialAttach(const UINT slotOffset, const MaterialTag& arg_materialTag)override;
		void SetShadowCamera(const unsigned int arg_layer, std::shared_ptr<ICamera> arg_shp_camera)override;
		void SetShadowTexture(const UINT arg_layer, TextureTag arg_shadowTex)override;
		TextureTag GetShadowTexture(const UINT arg_layer)override;
		std::shared_ptr<ICamera> GetShadowCamera(const unsigned int arg_layer)override;
		std::vector< std::shared_ptr<IDrawObject>> GetHitDrawObjects(std::shared_ptr<Collision::CollisionPrimitive> arg_prim,const int arg_layer)override;
		UINT* RegistDrawObject(std::shared_ptr< IDrawObject> arg_wkp_drawObject, const bool arg_afterDraw,const UINT arg_layer=0,const bool isShadow=false)override;
		void ShowUI()override;
		void UnRegistDrawObject(UINT* arg_index, const bool arg_afterDraw, const UINT arg_layer = 0 ,const bool isShadow = false)override;
		void Release()override;
		void ReleaseFogBuffer()override;
		void UpdateFog(const Fog& arg_param)override;
		std::shared_ptr<CBuffer_Dx12<Fog>> GetFogCBuffer()override;
	private:


		std::weak_ptr<GraphicDevice> wkp_graphicDevice;
		std::weak_ptr<IScene> wkp_iScene;
		std::weak_ptr<IResourceContainer>wkp_resourceContainer;
		std::vector<std::shared_ptr< DrawLayer>> vec_drawLayers;
		std::shared_ptr<CBuffer_Dx12<Fog>> CBuffer_fog;
	};
}
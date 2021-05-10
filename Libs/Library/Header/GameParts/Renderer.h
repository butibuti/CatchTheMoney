#pragma once
#include"stdafx.h"
#include"../Resources/DrawData/IDrawObject.h"
namespace ButiEngine {


	struct RegistCommand {
		bool isAfter;
		bool isShadow;
		std::shared_ptr< IDrawObject> shp_obj = nullptr;
		bool isRegist=false;
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
		void ShowUI()override;
		void RegistDrawObject(std::shared_ptr< IDrawObject> arg_wkp_drawObject, const bool arg_afterDraw,  const UINT arg_layer = 0, const bool isShadow = false)override;
		void UnRegistDrawObject(std::shared_ptr< IDrawObject> arg_shp_drawObject, const bool arg_afterDraw,  const UINT arg_layer = 0 ,const bool isShadow = false)override;
		void Release()override;
		void ReleaseFogBuffer()override;
		void UpdateFog(const Fog& arg_param)override;
		std::shared_ptr<CBuffer_Dx12<Fog>> GetFogCBuffer()override;
	private:


		std::weak_ptr<GraphicDevice> wkp_graphicDevice;
		std::weak_ptr<IScene> wkp_iScene;
		std::weak_ptr<IResourceContainer>wkp_resourceContainer;
		std::vector<std::shared_ptr< IDrawLayer>> vec_drawLayers;
		std::shared_ptr<CBuffer_Dx12<Fog>> CBuffer_fog;
	};
}
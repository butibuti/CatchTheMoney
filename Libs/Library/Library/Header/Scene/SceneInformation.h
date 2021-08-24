#pragma once

namespace ButiEngine {

	struct RenderingPathInfo :public IObject {
		void Initialize() {}
		void PreInitialize() {}
		virtual std::shared_ptr<IRenderingPath> CreateRenderingPath(std::shared_ptr<IRenderer> arg_shp_renderer,std::shared_ptr<GraphicDevice> arg_shp_graphicDevice) = 0;
		int order;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(order);
		}

	};
	struct SceneInformation :public IObject
	{
	public:
		SceneInformation(const std::string& arg_sceneName) {
			sceneName = arg_sceneName;
		}
		~SceneInformation();
		std::string GetSceneName() {
			return sceneName;
		}
		void Initialize()override {}
		void PreInitialize()override {}
	private:
		std::string sceneName;
	};
	struct SceneChangeInformation :public IObject
	{
	public:
		SceneChangeInformation() {
		}
		void Initialize()override {}
		void PreInitialize()override {}
	private:
	};
	struct SceneRenderingInformation 
	{
	public:
		SceneRenderingInformation() {
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(vec_renderingPathInfo);
			archive(vec_shadowCameraIndex);
			archive(vec_shadowTextures);
			archive(vec_staticShadowTextures);
			archive(layerCount);
		}
		std::vector<std::shared_ptr< RenderingPathInfo>> vec_renderingPathInfo;
		std::vector<std::vector<TextureTag>> vec_shadowTextures;
		std::vector<std::vector<TextureTag>> vec_staticShadowTextures;
		std::vector<int> vec_shadowCameraIndex;
		int layerCount = 1;
	private:
	};

	void OutputCereal(const std::shared_ptr<SceneRenderingInformation>& v, std::string arg_path);

	void InputCereal(std::shared_ptr<SceneRenderingInformation>& v,std::string arg_path);


}


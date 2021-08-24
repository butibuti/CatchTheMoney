#ifndef define RENDERINGPATH_H
#define RENDERINGPATH_H


namespace ButiEngine {

	struct ForwardCameraRenderingPathInfo :public RenderingPathInfo {
		std::shared_ptr<Transform> shp_cameraTrnasform;
		std::shared_ptr<CameraProjProperty> shp_cameraProp;
		std::shared_ptr<IRenderingPath> CreateRenderingPath(std::shared_ptr<IRenderer> arg_shp_renderer, std::shared_ptr<GraphicDevice> arg_shp_graphicDevice)override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shp_cameraTrnasform);
			archive(shp_cameraProp);
			archive(order);
		}
	};
	struct DeferredCameraRenderingPathInfo :public RenderingPathInfo {
		std::shared_ptr<Transform> shp_cameraTrnasform;
		std::shared_ptr<CameraProjProperty> shp_cameraProp;
		std::string forwardCameraPathName;
		std::shared_ptr<IRenderingPath> CreateRenderingPath(std::shared_ptr<IRenderer> arg_shp_renderer, std::shared_ptr<GraphicDevice> arg_shp_graphicDevice)override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shp_cameraTrnasform);
			archive(shp_cameraProp);
			archive(forwardCameraPathName);
			archive(order);
		}
	};

	class CameraRenderingPath :public IRenderingPath {
	public:
		std::shared_ptr<ICamera> GetCamera()const;
		void SetName(const std::string& arg_name);
		const std::string& GetName()const;
		void SetPlayActive()override;
		void SetEditActive()override;
	protected:
		std::shared_ptr<ICamera> shp_camera;
		std::shared_ptr<IScene> shp_scene;
	};

	class ForwardCameraRenderingPath :public CameraRenderingPath {
	public:
		ForwardCameraRenderingPath(std::shared_ptr<ICamera> arg_shp_camera, std::shared_ptr<IScene> arg_shp_scene);
		std::shared_ptr<RenderingPathInfo> GetRenderingPathInfo()override;
		void BefExecute()override;
		void Execute() override;
		void End() override;
		void Release() override;
		void OnShowGUI() override;
	private:
		std::shared_ptr<IRenderer> shp_renderer;
	};

	class DeferredCameraRenderingPath :public CameraRenderingPath {
	public:
		DeferredCameraRenderingPath(std::shared_ptr<ICamera> arg_shp_camera, std::shared_ptr<IScene> arg_shp_scene, std::string arg_forwardCamName);
		std::shared_ptr<RenderingPathInfo> GetRenderingPathInfo()override;
		void BefExecute()override;
		void Execute() override;
		void End() override;
		void Release() override;
		void OnShowGUI() override;
	private:
		std::shared_ptr<ICamera> shp_forwardCamera;
		std::shared_ptr<IRenderer> shp_renderer;
		std::string forwardCamName;
	};

}

CEREAL_REGISTER_TYPE(ButiEngine::RenderingPathInfo);
CEREAL_REGISTER_TYPE(ButiEngine::ForwardCameraRenderingPathInfo);
CEREAL_REGISTER_TYPE(ButiEngine::DeferredCameraRenderingPathInfo);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RenderingPathInfo, ButiEngine::ForwardCameraRenderingPathInfo);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RenderingPathInfo, ButiEngine::DeferredCameraRenderingPathInfo);
#endif // !define RENDERINGPATH_H


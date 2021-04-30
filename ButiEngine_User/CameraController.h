#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class CameraController :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "CameraController";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void ZoomIn();
		void ZoomOut();
	private:
		float zoomInFrame;
		float zoomOutFrame;
		float moveLength;
		float initCameraZ;
	};

}

BUTI_REGIST_GAMECOMPONENT(CameraController);
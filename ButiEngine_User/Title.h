#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class Title :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Title";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		static bool IsAnimation() { return isAnimation; }
		void GenerateBackGroundFrame();
	private:
		std::shared_ptr<PauseManager> shp_PauseManager;

		std::weak_ptr<GameObject> wkp_camera;
		std::weak_ptr<GameObject> wkp_fadeObject;
		std::weak_ptr<GameObject> wkp_titleLogo;
		std::shared_ptr<RelativeTimer> shp_FrameGenerateTimer;

		SoundTag bgm;
		SoundTag se_enter;
		SoundTag se_zoomOut;

		bool nextFlag;
		static bool isAnimation;
		bool isOnce;
		int nextSceneCount;
		int animationCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(Title);
#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class PauseManager;
	class Thanks :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Thanks";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI() override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);

		}
	private:
		void GenerateBackGroundFrame();
		std::shared_ptr<PauseManager> shp_PauseManager;


		std::weak_ptr<GameObject> wkp_fadeObject;
		std::weak_ptr<GameObject> wkp_thanksText, wkp_daikokutenFace, wkp_mouse;

		std::vector<std::weak_ptr<GameObject>> vec_apples;

		std::shared_ptr<GameObject> shp_appleParent;

		std::shared_ptr<RelativeTimer> shp_transitionTimer, shp_appleGather,shp_appleRotate, shp_appleMin,  shp_endTimer,shp_animationTimer, shp_FrameGenerateTimer, shp_appleGenerateTimer, shp_textGenerateTimer, shp_mouseGenerateTimer, shp_smileDaikokutenGenerateTimer;

		SoundTag bgm;
		SoundTag se_zoom;
		SoundTag se_exp;
		SoundTag se_apple;
		SoundTag se_enter;
		SoundTag se_dash;
		int appleCount;
		float appleRadius=0.0f,appleAngle=0;
		bool nextFlag,animationEnd=false;
		float bgmsize = 1.0f;
	};

}

BUTI_REGIST_GAMECOMPONENT(Thanks);
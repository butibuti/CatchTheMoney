#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class LogoSceneManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "LogoSceneManager";
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
			archive(shp_timer);
			archive(shp_transTimer);
			archive(logoApperanceSpeed);
		}
	private:
		std::shared_ptr<CBuffer<ObjectInformation>> shp_logoBuffer, shp_teamBuffer;
		std::shared_ptr<RelativeTimer> shp_timer,shp_logoTimer,shp_transTimer;
		float logoApperanceSpeed,logoTime,teamLogoTime=0.0f;
	};

}

BUTI_REGIST_GAMECOMPONENT(LogoSceneManager);
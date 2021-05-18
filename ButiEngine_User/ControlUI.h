#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class ControlUI :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ControlUI";
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
		void Stop() { stop = true; }
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;

		Vector3 initScale;
		float progress;
		bool animation;
		bool stop;

		void Animation();
		void OnNoPush();
		void OnPushAnyKey();
	};

}

BUTI_REGIST_GAMECOMPONENT(ControlUI);
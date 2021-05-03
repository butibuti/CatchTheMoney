#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class PanelManager;

	class FollowPanel :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "FollowPanel";
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
		std::weak_ptr<GameObject> GetClosestPanel() { return wkp_closestPanel; }
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<PanelManager> shp_panelManager;

		std::weak_ptr<GameObject> wkp_closestPanel;

		void StoreClosestPanel();
	};

}

BUTI_REGIST_GAMECOMPONENT(FollowPanel);
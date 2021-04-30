#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PanelManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "PanelManager";
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
		std::weak_ptr<GameObject> GetClosestPanel(float arg_otherX);

		void AddPanel(std::weak_ptr<GameObject> arg_panel);
	private:
		std::vector<std::weak_ptr<GameObject>> vec_panels;
	};

}

BUTI_REGIST_GAMECOMPONENT(PanelManager);
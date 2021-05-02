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

		void AddFrontPanel(std::weak_ptr<GameObject> arg_panel);
		void AddBackPanel(std::weak_ptr<GameObject> arg_panel);
		void RemoveGravityCores(int arg_num, float arg_gravity);
	private:
		std::vector<std::weak_ptr<GameObject>> vec_frontPanels;
		std::vector<std::weak_ptr<GameObject>> vec_backPanels;
		std::vector<std::weak_ptr<GameObject>> vec_panels;

		std::weak_ptr<GameObject> wkp_player;

		void StorePlayer();
		void SwapPanelNum(int arg_num1, int arg_num2);
	};

}

BUTI_REGIST_GAMECOMPONENT(PanelManager);
#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

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
		bool IsAnimation();
		void ResetMoveNum() { moveNum = 0; }
		void ResetMoveHistories() { vec_histories.clear(); }
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;

		std::vector<std::weak_ptr<GameObject>> vec_frontPanels;
		std::vector<std::weak_ptr<GameObject>> vec_backPanels;
		std::vector<std::weak_ptr<GameObject>> vec_panels;

		std::weak_ptr<GameObject> wkp_player;

		const int MOVE_LIMIT = 3;
		const bool RIGHT = true;
		const bool LEFT = false;
		int moveNum;
		std::vector<bool> vec_histories;
		int currentIndex;
		bool reset;

		void Contoroll();
		void StorePlayer();
		void SwapPanelNum(int arg_num1, int arg_num2);
		void SwapRight();
		void SwapLeft();
		void Undo();
		void Redo();
		void Reset();
		void RemoveHistories();
	};

}

BUTI_REGIST_GAMECOMPONENT(PanelManager);
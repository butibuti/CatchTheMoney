#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class ShakeComponent;
	class ReverseText;

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
		void ResetMoveNum()
		{
			moveNum = 0; 
			soundNum = 0;
		}
		void ResetMoveHistories() { vec_histories.clear(); }
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<ShakeComponent> shp_shake;
		std::shared_ptr<ReverseText> shp_reverseText;

		std::vector<std::weak_ptr<GameObject>> vec_frontPanels;
		std::vector<std::weak_ptr<GameObject>> vec_backPanels;
		std::vector<std::weak_ptr<GameObject>> vec_panels;

		std::weak_ptr<GameObject> wkp_target;

		SoundTag se_panelLimit;
		SoundTag se_slide0;
		SoundTag se_slide1;
		SoundTag se_slide2;

		const int MOVE_LIMIT = 6;
		const int SOUND_LIMIT = 3;
		const bool RIGHT = true;
		const bool LEFT = false;
		int moveNum;
		int soundNum;
		std::vector<bool> vec_histories;
		int currentIndex;
		bool reset;
		bool reverse;

		void Control();
		void StorePlayer();
		void SwapPanelNum(int arg_num1, int arg_num2, int arg_frame);
		void SwapRight(int arg_frame = 15);
		void SwapLeft(int arg_frame = 15);
		void Undo(int arg_frame = 15);
		void Redo(int arg_frame = 15);
		void Reset();
		void RemoveHistories();
		void PlaySlideSound();
	};

}

BUTI_REGIST_GAMECOMPONENT(PanelManager);
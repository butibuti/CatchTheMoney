#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Panel :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Panel";
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
		void SetPanelNum(int arg_num, bool arg_addAnimation, int arg_frame) 
		{
			panelNum = arg_num; 
			if (!arg_addAnimation) { return; }
			AddTransformAnimation(arg_frame);
		}
		int GetPanelNum() { return panelNum; }

		void SetParentPanel(std::weak_ptr<GameObject> arg_parentPanel)
		{
			wkp_parentPanel = arg_parentPanel;
		}
		std::weak_ptr<GameObject> GetParentPanel() { return wkp_parentPanel; }

		float GetGravity() { return gravity; }

		void AddGravityCore(int arg_num, float arg_gravity);
		void RemoveGravityCore(int arg_num, float arg_gravity);
		bool ContainsGravityCore(int arg_num);
		void ResetGravityCores();
		int GetGravityCoreCount() { return vec_gravityCoreNums.size(); }
		bool IsAnimation() { return animation; }

		void OnChangeGravity(bool arg_scroll);
		void SetDrawObjectSky(bool arg_back);
		void Lock(bool arg_back);
		bool IsLock() { return lock; }
		void SetDrawObjectTree(int arg_treeNum, bool arg_back);
	private:
		std::weak_ptr<GameObject> wkp_drawObjectGravity;
		std::weak_ptr<GameObject> wkp_drawObjectSky;
		std::weak_ptr<GameObject> wkp_drawObjectFrame;
		std::weak_ptr<GameObject> wkp_drawObjectLock;
		std::weak_ptr<GameObject> wkp_drawObjectTree;
		std::weak_ptr<GameObject> wkp_player;
		std::weak_ptr<GameObject> wkp_parentPanel;

		SoundTag se_cancel;

		int panelNum;
		float prevGravity;
		float currentGravity;
		float gravity;
		bool animation;
		bool scroll;
		bool lock;

		std::vector<int> vec_gravityCoreNums;

		void AddGravity(float arg_gravity) { gravity += arg_gravity; }
		void AddTransformAnimation(int arg_frame);
		void StorePlayer();
	};

}

BUTI_REGIST_GAMECOMPONENT(Panel);
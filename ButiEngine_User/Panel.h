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
		void SetPanelNum(int arg_num, bool arg_addAnimation) 
		{
			panelNum = arg_num; 
			if (!arg_addAnimation) { return; }
			AddTransformAnimation();
		}
		int GetPanelNum() { return panelNum; }
		void SetParentPanelNum(int arg_num) { parentPanelNum = arg_num; }
		int GetParentPanelNum() { return parentPanelNum; }

		float GetGravity() { return gravity; }

		void AddGravityCore(int arg_num, float arg_gravity);
		void RemoveGravityCore(int arg_num, float arg_gravity);
		bool ContainsGravityCore(int arg_num);
		void ResetGravityCores();
		int GetGravityCoreCount() { return vec_gravityCoreNums.size(); }
		bool IsAnimation() { return animation; }
	private:
		std::weak_ptr<GameObject> wkp_drawObject;

		int panelNum;
		int parentPanelNum;
		float gravity;
		bool animation;

		std::vector<int> vec_gravityCoreNums;

		void AddGravity(float arg_gravity) { gravity += arg_gravity; }
		void AddTransformAnimation();
	};

}

BUTI_REGIST_GAMECOMPONENT(Panel);
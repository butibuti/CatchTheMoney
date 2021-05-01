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
		void SetPanelNum(int arg_num) { panelNum = arg_num; }
		int GetPanelNum() { return panelNum; }
	private:
		std::weak_ptr<GameObject> wkp_drawObject;

		int panelNum;
	};

}

BUTI_REGIST_GAMECOMPONENT(Panel);
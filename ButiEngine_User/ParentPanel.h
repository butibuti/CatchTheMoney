#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParentPanel :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ParentPanel";
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
		void SetFrontPanel(std::weak_ptr<GameObject> arg_frontPanel);
		void SetBackPanel(std::weak_ptr<GameObject> arg_backPanel);
		void SetScale(const Vector3& arg_scale) { scale = arg_scale; }
	private:
		Vector3 scale;
		std::weak_ptr<GameObject> wkp_frontPanel;
		std::weak_ptr<GameObject> wkp_backPanel;

		int panelNum;

		void SetChildScale();
	};

}

BUTI_REGIST_GAMECOMPONENT(ParentPanel);
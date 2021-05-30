#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class TutorialSelect :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TutorialSelect";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void SetNowSelect(bool arg_select);
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		bool isSelect;
		bool isNowSelect;
	};

}

BUTI_REGIST_GAMECOMPONENT(TutorialSelect);
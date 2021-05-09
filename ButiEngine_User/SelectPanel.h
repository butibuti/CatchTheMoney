#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SelectPanel :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SelectPanel";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::shared_ptr<GameComponent> Clone()override;
		void SetStageNumbers(const int arg_front, const int arg_back);
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::weak_ptr<GameObject> wkp_parentSelectPanel;
		std::weak_ptr<GameObject> wkp_frontNumber;
		std::weak_ptr<GameObject> wkp_backNumber;

		int frontStageNum;
		int backStageNum;
		bool isOnce;

		void CreateNumber();
	};

}

BUTI_REGIST_GAMECOMPONENT(SelectPanel);
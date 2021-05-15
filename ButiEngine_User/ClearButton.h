#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ClearButton :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ClearButton";
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
		void Appear();
		void OnSelected();
		void OnEndSelect();
	private:
		Vector3 defaultScale;

		void AddAnimation(const Vector3& arg_targetScale);
	};

}

BUTI_REGIST_GAMECOMPONENT(ClearButton);
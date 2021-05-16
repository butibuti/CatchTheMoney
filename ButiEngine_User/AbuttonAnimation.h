#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class AbuttonAnimation :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "AbuttonAnimation";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		float currentScale;
		float previousScale;
		bool isChange;
		int changeCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(AbuttonAnimation);
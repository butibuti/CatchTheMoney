#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SitaSentan :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SitaSentan";
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
		void SetFrog(std::weak_ptr<GameObject> arg_frog) { wkp_frog = arg_frog; }
		std::weak_ptr<GameObject> GetFrog() { return wkp_frog; }
	private:
		std::weak_ptr<GameObject> wkp_frog;

		void SetX();
	};

}

BUTI_REGIST_GAMECOMPONENT(SitaSentan);
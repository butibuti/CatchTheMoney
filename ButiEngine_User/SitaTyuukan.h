#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SitaTyuukan :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SitaTyuukan";
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
		void SetSentan(std::weak_ptr<GameObject> arg_sentan) { wkp_sentan = arg_sentan; }
	private:
		std::weak_ptr<GameObject> wkp_frog;
		std::weak_ptr<GameObject> wkp_sentan;
	};

}

BUTI_REGIST_GAMECOMPONENT(SitaTyuukan);
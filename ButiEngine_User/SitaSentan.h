#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpliteAnimationComponent;
	class MobiusLoop;

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
		void SetZ();
		void SetAnim(int arg_anim);
		void SetAddZ(float arg_z) { addZ = arg_z; }
	private:
		std::weak_ptr<GameObject> wkp_frog;
		std::shared_ptr<MobiusLoop> shp_mobiusLoop;
		std::shared_ptr<SpliteAnimationComponent> shp_spriteAnimation;

		Vector3 initScale;
		float progress;
		int animationFrame;
		float addZ;

		void SetScale();
		void SetX();
		void Animation();
	};

}

BUTI_REGIST_GAMECOMPONENT(SitaSentan);
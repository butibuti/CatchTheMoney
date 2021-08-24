#pragma once
#include"MeshDrawComponent.h"
namespace ButiEngine {

	class SpliteAnimationComponent :public GameComponent {
	public:
		SpliteAnimationComponent() {}
		void OnUpdate()override;
		void OnShowUI()override;
		void Start()override;
		void OnSet()override;
		void UpdateHorizontalAnim(const int arg_updatePase);
		void UpdateVarticalAnim(const int arg_updatePase);
		void SetHorizontalAnim(const int arg_setNum) ;
		void SetVarticalAnim(const int arg_setNum);
		int GetHorizontalAnim() const;
		int GetVarticalAnim()const;
		int GetHorizontalSplitScale()const;
		int GetVarticalSplitScale()const;


		std::string GetGameComponentName()override {
			return "SpliteAnimationComponent";
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(horizontalNum);
			archive(varticalNum);
			archive( splitScale);
		}
		std::shared_ptr<GameComponent> Clone()override;
	private:
		void UpdateBuffer();
		Vector2 splitScale;
		int horizontalSplitScale;
		int varticalSplitScale;
		int horizontalNum;
		int varticalNum;
		std::shared_ptr<CBuffer<ObjectInformation>> shp_param;
	};
}


BUTI_REGIST_GAMECOMPONENT(SpliteAnimationComponent)
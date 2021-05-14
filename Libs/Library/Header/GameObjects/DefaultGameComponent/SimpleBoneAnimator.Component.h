#pragma once
#include"MeshDrawComponent.h"
namespace ButiEngine {

	class ModelAnimation;


	class ModelDrawData;
	class SimpleBoneAnimatorComponent :public GameComponent {
	public:
		SimpleBoneAnimatorComponent() {}
		void OnUpdate()override;
		void AddAnimation(std::shared_ptr<ModelAnimation> arg_shp_animation);
		void AddAnimation(MotionTag arg_motionTag);
		void SetLoop(const bool arg_isLoop);
		void OnShowUI()override;
		void Start()override;
		void SetModelData();
		std::string GetGameComponentName()override {
			return "SimpleBoneAnimatorComponent";
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(tag);
			archive(isLoop);
			archive(pase);
		}
		std::shared_ptr<GameComponent> Clone()override;
	private:
		std::shared_ptr<ModelAnimation> shp_animation;
		std::shared_ptr< ModelDrawData > shp_modelData;
		MotionTag tag;
		bool isLoop;
		float pase;
	};
}


BUTI_REGIST_GAMECOMPONENT(SimpleBoneAnimatorComponent)
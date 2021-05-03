#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class PanelManager;
	class FollowPanel;

	class GravityCore :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "GravityCore";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void SetCoreNum(int arg_num) { coreNum = arg_num; }
		int GetCoreNum() { return coreNum; }
		float GetGravity() { return gravity; }
		void SetGravity(float arg_gravity);
		bool GetGrabbed() { return grabbed; }
		void SetGrabbed(bool arg_flag) { grabbed = arg_flag; }
		void ReverseGravity() { gravity = -gravity; }
		void RemoveGravity();
		void AddGravity();
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<PanelManager> shp_panelManager;
		std::shared_ptr<FollowPanel> shp_followPanel;

		int coreNum;
		float gravity;
		bool grabbed;
	};

}

BUTI_REGIST_GAMECOMPONENT(GravityCore);
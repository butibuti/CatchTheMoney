#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PanelManager;

	class MapData
	{
	public:
		MapData(){}
		MapData(unsigned short arg_stageNum);
		Vector3 GetSize()
		{
			return Vector3(data[0].size(), data.size(), 0);
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(data);
		}
		std::vector<std::vector<int>> data;
	};

	class Map :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Map";
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
		static unsigned short GetStageNum() { return stageNum; }
		static void SetStageNum(unsigned short arg_stageNum) { stageNum = arg_stageNum; }
		void PutTile();
		void DestoryBlock();
	private:
		static unsigned short stageNum;

		std::shared_ptr<PanelManager> shp_panelManager;
	};

}

BUTI_REGIST_GAMECOMPONENT(Map);
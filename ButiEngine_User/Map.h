#pragma once
#include"Header/GameComponentHeader.h"
#include"ButiUtil/Util/CSVData.h"
namespace ButiEngine {

	class PanelManager;

	class MapData
	{
	public:
		MapData(){}
		MapData(unsigned short arg_stageNum);
		Vector2 GetSize()
		{
			return Vector2(shp_data->size_x, shp_data->size_y);
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
		}
		std::shared_ptr<CSVData> shp_data;
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
		void PutTile();
		void DestoryBlock();
	private:
		std::shared_ptr<PanelManager> shp_panelManager;
	};

}

BUTI_REGIST_GAMECOMPONENT(Map);
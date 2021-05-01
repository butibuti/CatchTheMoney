#include "stdafx_u.h"
#include "Map.h"
#include"GameSettings.h"
#include"ParentPanel.h"
#include"PanelManager.h"

unsigned short ButiEngine::Map::stageNum = 0;

void ButiEngine::Map::OnUpdate()
{
}

void ButiEngine::Map::OnSet()
{
}

void ButiEngine::Map::Start()
{
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
}

void ButiEngine::Map::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Map::Clone()
{
	return ObjectFactory::Create<Map>();
}

void ButiEngine::Map::PutTile()
{
	DestoryBlock();

	MapData mapData = MapData(stageNum);

	std::weak_ptr<GameObject> tile = std::shared_ptr<GameObject>();
	std::weak_ptr<GameObject> parentPanel = std::shared_ptr<GameObject>();
	std::weak_ptr<GameObject> frontPanel = std::shared_ptr<GameObject>();
	std::weak_ptr<GameObject> backPanel = std::shared_ptr<GameObject>();

	Vector3 mapSize = mapData.GetSize();
	Vector3 offset;
	offset.x = -GameSettings::windowWidth * 0.5f;
	offset.y = -GameSettings::windowHeight * 0.5f;
	offset.x += GameSettings::blockSize * 0.5f;
	offset.y += GameSettings::blockSize * 1.5f;

	Vector3 frameScale = Vector3(GameSettings::panelWidth, GameSettings::blockSize, 1.0f);

	const int panelWidthBlock = GameSettings::panelWidth / GameSettings::blockSize;

	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		if (x % panelWidthBlock == 0)
		{
			Vector3 panelPos;
			panelPos.x = -GameSettings::windowWidth * 0.5f + (x + panelWidthBlock * 0.5f) * GameSettings::blockSize;
			parentPanel = GetManager().lock()->AddObjectFromCereal("ParentPanel", ObjectFactory::Create<Transform>(panelPos, Vector3::Zero, 1.0f));
			shp_panelManager->AddParentPanel(parentPanel);
			auto parentPanelComponent = parentPanel.lock()->GetGameComponent<ParentPanel>();

			frontPanel = GetManager().lock()->AddObjectFromCereal("Panel", ObjectFactory::Create<Transform>(panelPos, Vector3::Zero, 1.0f));
			frontPanel.lock()->SetObjectName("FrontPanel");
			parentPanelComponent->SetFrontPanel(frontPanel);
			shp_panelManager->AddFrontPanel(frontPanel);


			Vector3 framePos = panelPos;
			framePos.y = -GameSettings::panelHeight * 0.5f + GameSettings::blockSize * 0.5f;
			tile = GetManager().lock()->AddObjectFromCereal("Floor", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));
			tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

			framePos.y *= -1.0f;
			tile = GetManager().lock()->AddObjectFromCereal("Ceiling", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));
			tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

			panelPos.x += GameSettings::windowWidth * 0.5f;
			backPanel = GetManager().lock()
				->AddObjectFromCereal("Panel", ObjectFactory::Create<Transform>(panelPos, Vector3::Zero, 1.0f));
			backPanel.lock()->SetObjectName("BackPanel");
			parentPanelComponent->SetBackPanel(backPanel);
			shp_panelManager->AddBackPanel(backPanel);

			framePos.x += GameSettings::windowWidth * 0.5f;
			tile = GetManager().lock()->AddObjectFromCereal("Floor", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));
			tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);

			framePos.y *= -1.0f;
			tile = GetManager().lock()->AddObjectFromCereal("Ceiling", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));
			tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
		}

		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			Vector3 position(x, y, 0);
			position *= GameSettings::blockSize;
			position += offset;
			position.y *= -1.0f;
			Vector3 scale(GameSettings::blockSize);
			scale.z = 1.0f;

			int mapChipID = mapData.data[y][x];

			if (mapChipID == GameSettings::air)
			{
			}
			else if (mapChipID == GameSettings::player)
			{
				Vector3 playerPos = position;
				tile = GetManager().lock()->AddObjectFromCereal("Player", ObjectFactory::Create<Transform>(playerPos, Vector3::Zero, scale));
			}
			else if (mapChipID == GameSettings::block)
			{
				tile = GetManager().lock()->AddObjectFromCereal("Block", ObjectFactory::Create<Transform>(position, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);
				Vector3 tmpPos = position;
				tmpPos.x += GameSettings::windowWidth * 0.5f;
				tmpPos.y *= -1.0f;
				tile = GetManager().lock()->AddObjectFromCereal("Block", ObjectFactory::Create<Transform>(tmpPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
			}
		}
	}
}

void ButiEngine::Map::DestoryBlock()
{
	auto tag = GetTagManager()->GetObjectTag("MapChip");
	auto manager = GetManager().lock();
	std::vector<std::shared_ptr<GameObject>> gameObjects = manager->GetGameObjects(tag);

	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->SetIsRemove(true);
	}
}

ButiEngine::MapData::MapData(unsigned short arg_stageNum)
{
	if (arg_stageNum == 0)
	{
		data =
		{
			//{2,0,0,0,0,0,0,0,0,0, 2,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,2,0,2, 2,0,0,0,0,0,0,0,0,2,},
			//{2,0,0,0,0,0,0,0,0,0, 2,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,2, 2,0,0,0,0,0,0,0,0,2,},
			//{2,0,0,0,0,0,0,0,0,0, 2,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,2, 2,0,0,0,0,0,0,0,0,2,},
			//{2,0,0,0,0,0,0,0,0,0, 2,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,2, 2,0,0,0,0,0,0,0,0,2,},
			//{2,0,0,0,0,0,0,0,0,0, 2,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,2, 2,0,0,0,0,0,0,0,0,2,},
			//{2,1,0,0,0,0,0,0,0,0, 2,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,2, 2,0,0,0,0,0,0,0,0,2,},

			//{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,},
			//{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,},
			//{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,},
			//{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,},
			//{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,},
			//{0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,},

			{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,},
			{0,0,0,0,2,0,0,0,0,0, 0,0,0,2,2,2,0,0,0,0, 0,0,0,2,2,2,0,0,0,0, 0,0,0,2,0,2,0,0,0,0,},
			{0,0,0,0,2,0,0,0,0,0, 0,0,0,0,0,2,0,0,0,0, 0,0,0,0,0,2,0,0,0,0, 0,0,0,2,0,2,0,0,0,0,},
			{0,0,0,0,2,0,0,0,0,0, 0,0,0,2,2,2,0,0,0,0, 0,0,0,2,2,2,0,0,0,0, 0,0,0,2,2,2,0,0,0,0,},
			{0,0,0,0,2,0,0,0,0,0, 0,0,0,2,0,0,0,0,0,0, 0,0,0,0,0,2,0,0,0,0, 0,0,0,0,0,2,0,0,0,0,},
			{0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,},
		};
	}
}

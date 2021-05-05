#include "stdafx_u.h"
#include "Map.h"
#include"GameSettings.h"
#include"ParentPanel.h"
#include"PanelManager.h"
#include"GravityCore.h"
#include"StageSelect.h"

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

	MapData mapData = MapData(StageSelect::GetStageNum());

	std::weak_ptr<GameObject> tile = std::shared_ptr<GameObject>();
	std::weak_ptr<GameObject> parentPanel = std::shared_ptr<GameObject>();
	std::weak_ptr<GameObject> frontPanel = std::shared_ptr<GameObject>();
	std::weak_ptr<GameObject> backPanel = std::shared_ptr<GameObject>();

	Vector2 mapSize = mapData.GetSize();
	Vector3 offset;
	offset.x = -GameSettings::windowWidth * 0.5f;
	offset.y = -GameSettings::windowHeight * 0.5f;
	offset.x += GameSettings::blockSize * 0.5f;
	offset.y += GameSettings::blockSize * 1.5f;

	Vector3 frameScale = Vector3(GameSettings::panelWidth, GameSettings::blockSize, 1.0f);

	const int panelWidthBlock = GameSettings::panelWidth / GameSettings::blockSize;

	int coreCount = 0;

	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		if (x % panelWidthBlock == 0)
		{
			Vector3 panelPos;
			panelPos.x = -GameSettings::windowWidth * 0.5f + (x + panelWidthBlock * 0.5f) * GameSettings::blockSize;
			panelPos.z = 0.0f;
			parentPanel = GetManager().lock()->AddObjectFromCereal("ParentPanel", ObjectFactory::Create<Transform>(panelPos, Vector3::Zero, 1.0f));
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
			frameScale.y *= -1;
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

			int mapChipID = mapData.shp_data->data[x][y];

			if (mapChipID == GameSettings::air)
			{
			}
			else if (mapChipID == GameSettings::player)
			{
				Vector3 playerPos = position;
				playerPos.z = -0.5f;
				tile = GetManager().lock()->AddObjectFromCereal("Player", ObjectFactory::Create<Transform>(playerPos, Vector3::Zero, scale));
			}
			else if (mapChipID == GameSettings::block)
			{
				Vector3 blockPos = position;
				blockPos.z = -0.1f;
				tile = GetManager().lock()->AddObjectFromCereal("Block", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

				blockPos.x += GameSettings::windowWidth * 0.5f;
				blockPos.y *= -1.0f;
				tile = GetManager().lock()->AddObjectFromCereal("Block", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
			}
			else if (mapChipID == GameSettings::goal)
			{
				Vector3 goalPos = position;
				goalPos.z = -0.1f;
				tile = GetManager().lock()->AddObjectFromCereal("Goal", ObjectFactory::Create<Transform>(goalPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);
				goalPos.x += GameSettings::windowWidth * 0.5f;
				goalPos.y *= -1.0f;
				tile = GetManager().lock()->AddObjectFromCereal("Goal", ObjectFactory::Create<Transform>(goalPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
			}
			else if (mapChipID == GameSettings::coreUp)
			{
				Vector3 corePos = position;
				corePos.z = -0.3f - 0.001f * coreCount;
				tile = GetManager().lock()->AddObjectFromCereal("GravityCore", ObjectFactory::Create<Transform>(corePos, Vector3::Zero, scale));
				auto core = tile.lock()->GetGameComponent<GravityCore>();
				core->SetGravity(0.2f);
				core->SetCoreNum(coreCount);
				coreCount++;
			}
			else if (mapChipID == GameSettings::coreDown)
			{
				Vector3 corePos = position;
				corePos.z = -0.3f - 0.001f * coreCount;
				tile = GetManager().lock()->AddObjectFromCereal("GravityCore", ObjectFactory::Create<Transform>(corePos, Vector3::Zero, scale));
				auto core = tile.lock()->GetGameComponent<GravityCore>();
				core->SetGravity(-0.2f);
				core->SetCoreNum(coreCount);
				coreCount++;
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
	shp_data = CSVReader::GetMatrix(GlobalSettings::GetResourceDirectory() + "MapData/Stage" + std::to_string(arg_stageNum) + ".csv");
}

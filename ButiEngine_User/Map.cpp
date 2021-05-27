#include "stdafx_u.h"
#include "Map.h"
#include"GameSettings.h"
#include"ParentPanel.h"
#include"PanelManager.h"
#include"GravityCore.h"
#include"StageSelect.h"
#include"Panel.h"
#include"Frog.h"

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

	MapData mapData;
	if (!GameSettings::isTitle)
	{
		mapData = MapData(StageSelect::GetStageNum());
	}
	else
	{
		mapData = MapData(100);
	}

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

	const int panelWidthBlock = GameSettings::panelWidth / GameSettings::blockSize;

	int coreCount = 0;
	int panelCount = 0;

	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		if (x % panelWidthBlock == 0)
		{
			Vector3 panelPos;
			panelPos.x = -GameSettings::windowWidth * 0.5f + (x + panelWidthBlock * 0.5f) * GameSettings::blockSize;
			panelPos.z = GameSettings::panelZ;
			parentPanel = GetManager().lock()->AddObjectFromCereal("ParentPanel", ObjectFactory::Create<Transform>(panelPos, Vector3::Zero, 1.0f));
			auto parentPanelComponent = parentPanel.lock()->GetGameComponent<ParentPanel>();

			frontPanel = GetManager().lock()->AddObjectFromCereal("Panel", ObjectFactory::Create<Transform>(panelPos, Vector3::Zero, 1.0f));
			frontPanel.lock()->SetObjectName("Panel_Front");

			auto fPanel = frontPanel.lock()->GetGameComponent<Panel>();
			fPanel->SetDrawObjectSky(false);
			fPanel->SetDrawObjectTree(panelCount, false);
			parentPanelComponent->SetFrontPanel(frontPanel);
			shp_panelManager->AddFrontPanel(frontPanel);

			Vector3 frameScale = Vector3(GameSettings::panelWidth, GameSettings::blockSize, 1.0f);

			Vector3 framePos = panelPos;
			framePos.y = -GameSettings::panelHeight * 0.5f + GameSettings::blockSize * 0.5f;
			tile = GetManager().lock()->AddObjectFromCereal("Ground_Front", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));
			tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

			framePos.y *= -1.0f;
			tile = GetManager().lock()->AddObjectFromCereal("Cloud_Front", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));
			tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

			panelPos.x += GameSettings::windowWidth * 0.5f;
			backPanel = GetManager().lock()
				->AddObjectFromCereal("Panel", ObjectFactory::Create<Transform>(panelPos, Vector3::Zero, 1.0f));
			backPanel.lock()->SetObjectName("Panel_Back");


			auto frontSidePanel = GetManager().lock()->AddObjectFromCereal("Ground_side");
			frontSidePanel.lock()->transform->SetBaseTransform(frontPanel.lock()->transform, true);
			auto backSidePanel = GetManager().lock()->AddObjectFromCereal("Sky_side");
			backSidePanel.lock()->transform->SetBaseTransform(backPanel.lock()->transform, true);

			auto bPanel = backPanel.lock()->GetGameComponent<Panel>();
			bPanel->SetDrawObjectSky(true);
			bPanel->SetDrawObjectTree(panelCount, true);
			parentPanelComponent->SetBackPanel(backPanel);
			shp_panelManager->AddBackPanel(backPanel);

			int panelID = mapData.shp_data->data[x][0];
			if (panelID >= GameSettings::lockPanel)
			{
				frontPanel.lock()->GetGameComponent<Panel>()->Lock(false);
				backPanel.lock()->GetGameComponent<Panel>()->Lock(true);
			}

			panelCount++;

			framePos.x += GameSettings::windowWidth * 0.5f;
			frameScale.y *= -1;
			tile = GetManager().lock()->AddObjectFromCereal("Ground_Back", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));
			tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);

			framePos.y *= -1.0f;
			tile = GetManager().lock()->AddObjectFromCereal("Cloud_Back", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));
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
			int onceOfID = mapChipID % GameSettings::lockPanel;

			if (onceOfID == GameSettings::air)
			{
			}
			else if (onceOfID == GameSettings::player)
			{
				Vector3 playerPos = position;
				playerPos.z = GameSettings::playerZ;
				tile = GetManager().lock()->AddObjectFromCereal("Player", ObjectFactory::Create<Transform>(playerPos, Vector3::Zero, scale));
			}
			else if (onceOfID == GameSettings::block)
			{
				Vector3 blockPos = position;
				blockPos.z = GameSettings::blockZ;
				tile = GetManager().lock()->AddObjectFromCereal("Block_Front", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

				blockPos.x += GameSettings::windowWidth * 0.5f;
				blockPos.y *= -1.0f;
				tile = GetManager().lock()->AddObjectFromCereal("Block_Back", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
			}
			else if (onceOfID == GameSettings::goalUp || onceOfID == GameSettings::goalDown)
			{
				Vector3 goalPos = position;
				goalPos.z = GameSettings::goalZ;
				Vector3 tmpScale = scale;
				if (onceOfID == GameSettings::goalDown)
				{
					tmpScale.y *= -1;
				}
				tile = GetManager().lock()->AddObjectFromCereal("Goal", ObjectFactory::Create<Transform>(goalPos, Vector3::Zero, tmpScale*2));
				tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);
				goalPos.x += GameSettings::windowWidth * 0.5f;
				goalPos.y *= -1.0f;
				tmpScale.y *= -1;
				tile = GetManager().lock()->AddObjectFromCereal("Goal", ObjectFactory::Create<Transform>(goalPos, Vector3::Zero, tmpScale*2));
				tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
			}
			else if (onceOfID == GameSettings::coreUp)
			{
				Vector3 corePos = position;
				corePos.z = GameSettings::coreZ - 0.001f * coreCount;
				tile = GetManager().lock()->AddObjectFromCereal("GravityCore", ObjectFactory::Create<Transform>(corePos, Vector3::Zero, scale));
				auto core = tile.lock()->GetGameComponent<GravityCore>();
				core->SetGravity(GameSettings::gravity);
				core->SetCoreNum(coreCount);
				coreCount++;
			}
			else if (onceOfID == GameSettings::coreDown)
			{
				Vector3 corePos = position;
				corePos.z = GameSettings::coreZ - 0.001f * coreCount;
				tile = GetManager().lock()->AddObjectFromCereal("GravityCore", ObjectFactory::Create<Transform>(corePos, Vector3::Zero, scale));
				auto core = tile.lock()->GetGameComponent<GravityCore>();
				core->SetGravity(-GameSettings::gravity);
				core->SetCoreNum(coreCount);
				coreCount++;
			}
			else if (onceOfID == GameSettings::fill)
			{
				Vector3 blockPos = position;
				blockPos.x += GameSettings::panelWidth * 0.5f - GameSettings::blockSize * 0.5f;
				blockPos.y += -GameSettings::panelHeight * 0.5f + GameSettings::blockSize * 1.5f;
				blockPos.z = GameSettings::blockZ;

				Vector3 blockScale = Vector3::Zero;
				blockScale.x = GameSettings::panelWidth;
				blockScale.y = GameSettings::panelHeight - GameSettings::blockSize * 2.0f;
				blockScale.z = 1.0f;

				tile = GetManager().lock()->AddObjectFromCereal("FillBlock_Front", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, blockScale));
				tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

				blockPos.x += GameSettings::windowWidth * 0.5f;
				blockPos.y *= -1.0f;
				tile = GetManager().lock()->AddObjectFromCereal("FillBlock_Back", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, blockScale));
				tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
			}
			else if (onceOfID == GameSettings::blockTop)
			{
				Vector3 blockPos = position;
				blockPos.z = GameSettings::blockZ;
				tile = GetManager().lock()->AddObjectFromCereal("Block_FrontTop", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

				blockPos.x += GameSettings::windowWidth * 0.5f;
				blockPos.y *= -1.0f;
				tile = GetManager().lock()->AddObjectFromCereal("Block_BackTop", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
			}
			else if (onceOfID == GameSettings::blockBottom)
			{
				Vector3 blockPos = position;
				blockPos.z = GameSettings::blockZ;
				tile = GetManager().lock()->AddObjectFromCereal("Block_FrontBottom", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

				blockPos.x += GameSettings::windowWidth * 0.5f;
				blockPos.y *= -1.0f;
				tile = GetManager().lock()->AddObjectFromCereal("Block_BackBottom", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
			}
			else if (onceOfID == GameSettings::blockReversible)
			{
				Vector3 blockPos = position;
				blockPos.z = GameSettings::blockZ;
				tile = GetManager().lock()->AddObjectFromCereal("Block_FrontReversible", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(frontPanel.lock()->transform);

				blockPos.x += GameSettings::windowWidth * 0.5f;
				blockPos.y *= -1.0f;
				tile = GetManager().lock()->AddObjectFromCereal("Block_BackReversible", ObjectFactory::Create<Transform>(blockPos, Vector3::Zero, scale));
				tile.lock()->transform->SetBaseTransform(backPanel.lock()->transform);
			}
			else if (onceOfID == GameSettings::frogTopRight)
			{
				Vector3 frogPos = position;
				frogPos.z = GameSettings::frogZ;
				Vector3 frogScale = scale * 2.0f;
				auto front = GetManager().lock()->AddObjectFromCereal("Frog", ObjectFactory::Create<Transform>(frogPos, Vector3::Zero, frogScale));

				frogPos.x += GameSettings::windowWidth * 0.5f;
				frogPos.y *= -1.0f;
				frogScale.y *= -1.0f;
				auto back = GetManager().lock()->AddObjectFromCereal("Frog", ObjectFactory::Create<Transform>(frogPos, Vector3::Zero, frogScale));

				auto front_frog = front.lock()->GetGameComponent<Frog>();
				auto back_frog = back.lock()->GetGameComponent<Frog>();

				front_frog->SetBackFrog(back);
				back_frog->SetBackFrog(front);

				front_frog->SetDefaultGravity(true);
				back_frog->SetDefaultGravity(true);
			}
			else if (onceOfID == GameSettings::frogTopLeft)
			{
				Vector3 frogPos = position;
				frogPos.z = GameSettings::frogZ;
				Vector3 frogScale = scale * 2.0f;
				frogScale.x *= -1.0f;
				auto front = GetManager().lock()->AddObjectFromCereal("Frog", ObjectFactory::Create<Transform>(frogPos, Vector3::Zero, frogScale));

				frogPos.x += GameSettings::windowWidth * 0.5f;
				frogPos.y *= -1.0f;
				frogScale.y *= -1.0f;
				auto back = GetManager().lock()->AddObjectFromCereal("Frog", ObjectFactory::Create<Transform>(frogPos, Vector3::Zero, frogScale));

				auto front_frog = front.lock()->GetGameComponent<Frog>();
				auto back_frog = back.lock()->GetGameComponent<Frog>();

				front_frog->SetBackFrog(back);
				back_frog->SetBackFrog(front);

				front_frog->SetDefaultGravity(true);
				back_frog->SetDefaultGravity(true);
			}
			else if (onceOfID == GameSettings::frogBottomRight)
			{
				Vector3 frogPos = position;
				frogPos.z = GameSettings::frogZ;
				Vector3 frogScale = scale * 2.0f;
				frogScale.y *= -1.0f;
				auto front = GetManager().lock()->AddObjectFromCereal("Frog", ObjectFactory::Create<Transform>(frogPos, Vector3::Zero, frogScale));

				frogPos.x += GameSettings::windowWidth * 0.5f;
				frogPos.y *= -1.0f;
				frogScale.y *= -1.0f;
				auto back = GetManager().lock()->AddObjectFromCereal("Frog", ObjectFactory::Create<Transform>(frogPos, Vector3::Zero, frogScale));

				auto front_frog = front.lock()->GetGameComponent<Frog>();
				auto back_frog = back.lock()->GetGameComponent<Frog>();

				front_frog->SetBackFrog(back);
				back_frog->SetBackFrog(front);

				front_frog->SetDefaultGravity(false);
				back_frog->SetDefaultGravity(false);
			}
			else if (onceOfID == GameSettings::frogBottomLeft)
			{
				Vector3 frogPos = position;
				frogPos.z = GameSettings::frogZ;
				Vector3 frogScale = scale * 2.0f;
				frogScale.x *= -1.0f;
				frogScale.y *= -1.0f;
				auto front = GetManager().lock()->AddObjectFromCereal("Frog", ObjectFactory::Create<Transform>(frogPos, Vector3::Zero, frogScale));

				frogPos.x += GameSettings::windowWidth * 0.5f;
				frogPos.y *= -1.0f;
				frogScale.y *= -1.0f;
				auto back = GetManager().lock()->AddObjectFromCereal("Frog", ObjectFactory::Create<Transform>(frogPos, Vector3::Zero, frogScale));

				auto front_frog = front.lock()->GetGameComponent<Frog>();
				auto back_frog = back.lock()->GetGameComponent<Frog>();

				front_frog->SetBackFrog(back);
				back_frog->SetBackFrog(front);

				front_frog->SetDefaultGravity(false);
				back_frog->SetDefaultGravity(false);
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

#include "stdafx_u.h"
#include "Map.h"
#include"GameSettings.h"

unsigned short ButiEngine::Map::stageNum = 0;

void ButiEngine::Map::OnUpdate()
{
}

void ButiEngine::Map::OnSet()
{
}

void ButiEngine::Map::Start()
{
}

void ButiEngine::Map::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Map::Clone()
{
	return ObjectFactory::Create<Map>();
}

void ButiEngine::Map::PutBlock()
{
	DestoryBlock();

	MapData mapData = MapData(stageNum);

	Vector3 mapSize = mapData.GetSize();
	Vector3 offset;
	offset.x = -GameSettings::windowWidth * 0.5f;
	offset.y = -GameSettings::windowHeight * 0.5f;
	offset.x += GameSettings::blockSize * 0.5f;
	offset.y += GameSettings::blockSize * 1.5f;

	const int panelWidth = GameSettings::windowWidth / 8;
	const int panelHeight = GameSettings::windowHeight;
	Vector3 frameScale = Vector3(panelWidth, GameSettings::blockSize, 1.0f);

	for (unsigned int x = 0; x < mapSize.x; x++)
	{

		if (x % 5 == 0 && x != 0)
		{
			Vector3 framePos;
			framePos.x = -GameSettings::windowWidth * 0.5f + x * GameSettings::blockSize;
			framePos.y = -panelHeight * 0.5f + GameSettings::blockSize * 0.5f;
			gameObject = GetManager().lock()->AddObjectFromCereal("Floor", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));

			framePos.y *= -1.0f;
			gameObject = GetManager().lock()->AddObjectFromCereal("Ceiling", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));

			framePos.x += GameSettings::windowWidth * 0.5f;
			gameObject = GetManager().lock()->AddObjectFromCereal("Floor", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));

			framePos.y *= -1.0f;
			gameObject = GetManager().lock()->AddObjectFromCereal("Ceiling", ObjectFactory::Create<Transform>(framePos, Vector3::Zero, frameScale));
		}

		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			Vector3 position(x, y, 0);
			position *= GameSettings::blockSize;
			position += offset;
			position.y *= -1.0f;
			Vector3 scale(GameSettings::blockSize);
			scale.z = 1.0f;
			std::weak_ptr<GameObject> gameObject = std::shared_ptr<GameObject>();

			int mapChipID = mapData.data[y][x];

			if (mapChipID == GameSettings::air)
			{
			}
			else if (mapChipID == GameSettings::player)
			{
				gameObject = GetManager().lock()->AddObjectFromCereal("Player", ObjectFactory::Create<Transform>(position, Vector3::Zero, scale));
			}
			else if (mapChipID == GameSettings::block)
			{
				gameObject = GetManager().lock()->AddObjectFromCereal("Block", ObjectFactory::Create<Transform>(position, Vector3::Zero, scale));
				Vector3 tmpPos = position;
				tmpPos.x += GameSettings::windowWidth * 0.5f;
				tmpPos.y *= -1.0f;
				gameObject = GetManager().lock()->AddObjectFromCereal("Block", ObjectFactory::Create<Transform>(tmpPos, Vector3::Zero, scale));
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
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
			{2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,},
			{2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,},
			{2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,},
			{2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,},
			{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
		};
	}
}

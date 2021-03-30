#include "stdafx_u.h"
#include "Block.h"
#include"GameSettings.h"

void ButiEngine::Block::OnUpdate()
{
	Create();
}

void ButiEngine::Block::OnSet()
{
}

void ButiEngine::Block::Start()
{
	createFinished = false;
}

void ButiEngine::Block::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Block::Clone()
{
	return ObjectFactory::Create<Block>();
}

void ButiEngine::Block::Create()
{
	if (createFinished)
	{
		return;
	}

	Correction();

	Vector2 midPoint = (drawStartPoint + drawEndPoint) / 2.0f;
	gameObject.lock()->transform->SetWorldPosition(Vector3(midPoint.x, midPoint.y, 1.0f));

	Vector2 scale = drawEndPoint - drawStartPoint;
	gameObject.lock()->transform->SetLocalScale(Vector3(scale.x, scale.y, 1.0f).Abs());
}

void ButiEngine::Block::Correction()
{
	Vector2 startBlockPos = startPoint / GameSettings::gridSize;
	Vector2 endBlockPos = endPoint / GameSettings::gridSize;
	startBlockPos.x = (int)startBlockPos.x;
	startBlockPos.y = (int)startBlockPos.y;
	endBlockPos.x = (int)endBlockPos.x;
	endBlockPos.y = (int)endBlockPos.y;

	if (endBlockPos.x > startBlockPos.x)
	{
		drawStartPoint.x = startBlockPos.x * GameSettings::gridSize;
		drawEndPoint.x = (endBlockPos.x + 1) * GameSettings::gridSize;
	}
	else
	{
		drawStartPoint.x = (startBlockPos.x + 1) * GameSettings::gridSize;
		drawEndPoint.x = endBlockPos.x * GameSettings::gridSize;
	}

	if (endBlockPos.y > startBlockPos.y)
	{
		drawStartPoint.y = (startBlockPos.y - 1) * GameSettings::gridSize;
		drawEndPoint.y = endBlockPos.y * GameSettings::gridSize;
	}
	else
	{
		drawStartPoint.y = startBlockPos.y * GameSettings::gridSize;
		drawEndPoint.y = (endBlockPos.y - 1) * GameSettings::gridSize;
	}
}

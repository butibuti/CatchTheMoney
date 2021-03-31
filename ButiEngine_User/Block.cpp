#include "stdafx_u.h"
#include "Block.h"
#include"GameSettings.h"

void ButiEngine::Block::OnUpdate()
{
	Create();
	OnOutScreen();
}

void ButiEngine::Block::OnSet()
{
}

void ButiEngine::Block::Start()
{
	createFinished = false;
	createable = true;
	gameObject.lock()->RegistReactionComponent(GetThis<GameComponent>());
}

void ButiEngine::Block::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Block::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	auto tag = GetTagManager()->GetObjectTag("ImpossibleCreateBlockArea");
	if (arg_other.lock()->HasGameObjectTag(tag))
	{
		createable = false;
		auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
		meshDraw->SetMaterialTag(gameObject.lock()->GetResourceContainer()->GetMaterialTag("orange"));
		meshDraw->ReRegist();
	}
}

void ButiEngine::Block::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
	auto tag = GetTagManager()->GetObjectTag("ImpossibleCreateBlockArea");
	if (arg_other.lock()->HasGameObjectTag(tag))
	{
		createable = true;
		auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
		meshDraw->SetMaterialTag(gameObject.lock()->GetResourceContainer()->GetMaterialTag("BlockMaterial"));
		meshDraw->ReRegist();
	}
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Block::Clone()
{
	return ObjectFactory::Create<Block>();
}

void ButiEngine::Block::CreateClone()
{
	if (clone)
	{
		return;
	}
	wkp_clone = GetManager().lock()->AddObjectFromCereal("Block");
	wkp_clone.lock()->GetGameComponent<Block>()->SetClone(true);
	wkp_clone.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_clone.lock()->transform->SetLocalPosition(Vector3::Zero);
	wkp_clone.lock()->transform->TranslateX(-GameSettings::windowWidth / gameObject.lock()->transform->GetLocalScale().x);
	wkp_clone.lock()->transform->SetLocalScale(1.0f);
}

void ButiEngine::Block::FinishCreate()
{
	if (!createable)
	{
		gameObject.lock()->SetIsRemove(true);
		return;
	}
	createFinished = true;
	gameObject.lock()->transform->TranslateZ(0.1f);
	CreateClone();
	auto tag = GetTagManager()->CreateGameObjectTag("ImpossibleCreateBlockArea");
	gameObject.lock()->SetGameObjectTag(tag);
}

void ButiEngine::Block::Create()
{
	if (clone || createFinished)
	{
		return;
	}

	Correction();

	Vector2 midPoint = (drawStartPoint + drawEndPoint) / 2.0f;
	gameObject.lock()->transform->SetWorldPosition(Vector3(midPoint.x, midPoint.y, 0.9f));

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

void ButiEngine::Block::OnOutScreen()
{
	bool outScreen = false;
	float left = gameObject.lock()->transform->GetWorldPosition().x - gameObject.lock()->transform->GetLocalScale().x / 2.0f;

	if (left > GameSettings::windowWidth)
	{
		gameObject.lock()->transform->SetWorldPosition(wkp_clone.lock()->transform->GetWorldPosition());
	}
}

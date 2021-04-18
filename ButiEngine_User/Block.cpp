#include "stdafx_u.h"
#include "Block.h"
#include"GameSettings.h"
#include"PauseManager.h"

void ButiEngine::Block::OnUpdate()
{
}

void ButiEngine::Block::OnSet()
{
}

void ButiEngine::Block::Start()
{
}

void ButiEngine::Block::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Block::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Block::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Block::Clone()
{
	return ObjectFactory::Create<Block>();
}

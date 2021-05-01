#include "stdafx_u.h"
#include "PredictionLine.h"
#include "Player.h"

void ButiEngine::PredictionLine::OnUpdate()
{

}

void ButiEngine::PredictionLine::OnSet()
{
}

void ButiEngine::PredictionLine::Start()
{
}

void ButiEngine::PredictionLine::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::PredictionLine::Clone()
{
	return ObjectFactory::Create<PredictionLine>();
}

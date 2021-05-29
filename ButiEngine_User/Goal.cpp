#include "stdafx_u.h"
#include "Goal.h"

void ButiEngine::Goal::OnUpdate()
{
}

void ButiEngine::Goal::OnSet()
{
}

void ButiEngine::Goal::Start()
{
}

void ButiEngine::Goal::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Goal::Clone()
{
	return ObjectFactory::Create<Goal>();
}

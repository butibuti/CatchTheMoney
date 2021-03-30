#include "stdafx_u.h"
#include "Goal.h"
#include"MobiusMover.h"

void ButiEngine::Goal::OnUpdate()
{
	const float speed = 1.0f;
	progress += speed;
	gameObject.lock()->GetGameComponent<MobiusMover>()->SetProgress(progress);
}

void ButiEngine::Goal::OnSet()
{
}

void ButiEngine::Goal::Start()
{
	progress = 360.0f;
}

void ButiEngine::Goal::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Goal::Clone()
{
	return ObjectFactory::Create<Goal>();
}

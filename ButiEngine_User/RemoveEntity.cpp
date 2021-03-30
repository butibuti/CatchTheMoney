#include "stdafx_u.h"
#include "RemoveEntity.h"

void ButiEngine::RemoveEntity::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::R))
	{
		auto entityTag = GetTagManager()->GetObjectTag("Entity");
		auto entities = GetManager().lock()->GetGameObjects(entityTag);

		for (auto itr = entities.begin(); itr != entities.end(); ++itr)
		{
			(*itr)->SetIsRemove(true);
		}

		auto centerTag = GetTagManager()->GetObjectTag("Center");
		auto centers = GetManager().lock()->GetGameObjects(centerTag);

		for (auto itr = centers.begin(); itr != centers.end(); ++itr)
		{
			(*itr)->SetIsRemove(true);
		}
	}
}

void ButiEngine::RemoveEntity::OnSet()
{
}

void ButiEngine::RemoveEntity::Start()
{
}

void ButiEngine::RemoveEntity::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::RemoveEntity::Clone()
{
	return ObjectFactory::Create<RemoveEntity>();
}

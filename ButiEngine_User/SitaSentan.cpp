#include "stdafx_u.h"
#include "SitaSentan.h"

void ButiEngine::SitaSentan::OnUpdate()
{
}

void ButiEngine::SitaSentan::OnSet()
{
}

void ButiEngine::SitaSentan::Start()
{
}

void ButiEngine::SitaSentan::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SitaSentan::Clone()
{
	return ObjectFactory::Create<SitaSentan>();
}

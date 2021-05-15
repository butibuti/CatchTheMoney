#include "stdafx_u.h"
#include "TalkWindow.h"

void ButiEngine::TalkWindow::OnUpdate()
{
}

void ButiEngine::TalkWindow::OnSet()
{
}

void ButiEngine::TalkWindow::Start()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TalkWindow::Clone()
{
	return ObjectFactory::Create<TalkWindow>();
}

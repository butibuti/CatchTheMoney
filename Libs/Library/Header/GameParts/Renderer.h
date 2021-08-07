#pragma once
#include"stdafx.h"
#include"../Resources/DrawData/IDrawObject.h"
namespace ButiEngine {
std::shared_ptr<IRenderer> CreateRenderer(std::weak_ptr<IScene> arg_wkp_iscene);
}
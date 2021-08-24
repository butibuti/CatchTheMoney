#pragma once
#include"stdafx.h"
namespace ButiEngine {
/// <summary>
/// ResourceContainer�̍쐬
/// </summary>
/// <returns></returns>
std::shared_ptr<IResourceContainer> CreateResourceContainer();

void OutputCereal(const ResourceLoadData& v);
void InputCereal(ResourceLoadData& v);
}


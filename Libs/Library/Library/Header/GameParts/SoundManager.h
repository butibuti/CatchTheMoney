#pragma once
#include"stdafx.h"
#include"../Resources/WaveDatas.h"
namespace ButiEngine {

	std::shared_ptr<ISoundManager> CreateSoundManager(std::weak_ptr<IApplication> arg_wkp_app);

	
}


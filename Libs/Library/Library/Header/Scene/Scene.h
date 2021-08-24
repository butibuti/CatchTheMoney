#pragma once
#ifndef _Scene_H_
#define _Scene_H_
#include"stdafx.h"
namespace ButiEngine {

	std::shared_ptr<IScene> CreateScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, std::shared_ptr< SceneInformation> argSceneInformation);

}
#endif // !_Scene_H_


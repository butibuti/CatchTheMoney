#include<string>
#include"GameParts/GamePartsInterface.h"
namespace ButiEngine {
struct ApplicationInitData {
	std::string appWindowName="ButiApp";
	std::string initSceneName="InitScene";
	WindowPopType popType=WindowPopType::normal;
	int windowWidth=1920;
	int windowHeight=1080;
	bool isFullScreen = false;
};
ApplicationInitData InputApplicationInitData(const std::string& arg_filePath="ButiEngine.ini");
void OutputApplicationInitData(const ApplicationInitData& arg_outputData, const std::string& arg_filePath = "ButiEngine.ini");

std::shared_ptr<IApplication> CreateDefaultApplicationInstance(const ApplicationInitData& arg_appInitData);
std::shared_ptr<IApplication> CreateEditorApplicationInstance( const ApplicationInitData& arg_appInitData);

}
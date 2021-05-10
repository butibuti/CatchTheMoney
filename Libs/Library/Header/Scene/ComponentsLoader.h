#pragma once

#include "../GameObjects/GameComponent.h"
namespace ButiEngine {
	class ComponentsLoader :public IObject{
	public:

		static void CreateInstance();

		static  std::shared_ptr< ComponentsLoader> GetInstance();

		void Initialize(){}
		void PreInitialize(){}
		void Release();
		void AddGameComponent(std::shared_ptr<GameComponent> arg_gamecomponent);

		void RemoveComponent(const std::string& arg_name);

		std::shared_ptr<GameComponent> ShowAddGameComponentUI();

		template <typename T>
		void AddGameComponent() {
				AddGameComponent(ObjectFactory::Create<T>());
		}


		template <typename T>
		void RemoveComponent() {
			RemoveComponent(ObjectFactory::Create<T>()->GetGameComponentName());
		}


	private:
		
		std::map<std::string, std::shared_ptr<GameComponent>> map_addGameComponents;
		std::shared_ptr<GameComponent> shp_currentSelectObj;

		static std::shared_ptr< ComponentsLoader> instance;
	};


}
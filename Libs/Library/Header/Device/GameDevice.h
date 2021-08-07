#pragma once

#include"../Device/Input.h"
namespace ButiEngine {

	struct BlankSpace {
		UINT index;
		UINT size;
	};
	class GameDevice
	{
	public:
		~GameDevice();
		static void Initialize();
		static float WorldSpeed;
		static std::unique_ptr<Input>& GetInput();
		static void SetIsEnd(const bool arg_isEnd);
		static bool GetIsEnd();
		static void SetReturnCode(const int arg_returnCode);
		static int GetReturnCode();
	private:
		GameDevice();
		static std::unique_ptr<Input> input;
	};
}

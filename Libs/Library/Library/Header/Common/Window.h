#pragma once
#include"stdafx.h"

namespace ButiEngine {


	class Window:public IWindow
	{
	public:
		Window();
		void Initialize(const std::string arg_windowName,const WindowPopType arg_popType ,const bool isFullScreen, const UINT width=0,const UINT height = 0)override;
		HWND GetHandle()override;
		UInt2 GetSize()override;
		Vector2 GetWindowCenterPosition()override;
		void SetSize(int width, int height)override;
		bool Update()override;
		Vector2 GetWindowPosition()override;
		void Release()override;
	private:
		HWND handle;

		static LRESULT CALLBACK ProceedMessage(HWND window,	UINT message, WPARAM wParam, LPARAM lParam);
	};
}
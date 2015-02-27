#pragma once

#include "resource.h"

class utils {
public:
	inline static void mouseClick( int x, int y ) {
		POINT screen, old, now;
		screen.x = 65535.0f / ( GetSystemMetrics( SM_CXSCREEN )-1 ); 
		screen.y =  65535.0f / ( GetSystemMetrics( SM_CYSCREEN )-1 );
		now.x = x * screen.x;
		now.y = y * screen.y;
		GetCursorPos(&old);
		old.x = old.x * screen.x;
		old.y = old.y * screen.y;
		ShowCursor(FALSE);
		INPUT i[4];
		i[0].type = INPUT_MOUSE;
		i[0].mi.dx = now.x;
		i[0].mi.dy = now.y;
		i[0].mi.mouseData = 0;
		i[0].mi.time = 0;
		i[0].mi.dwExtraInfo = 0;
		i[0].mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
		i[1].type = INPUT_MOUSE;
		i[1].mi.dx = now.x;
		i[1].mi.dy = now.y;
		i[1].mi.mouseData = 0;
		i[1].mi.time = 0;
		i[1].mi.dwExtraInfo = 0;
		i[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		i[2].type = INPUT_MOUSE;
		i[2].mi.dx = now.x;
		i[2].mi.dy = now.y;
		i[2].mi.mouseData = 0;
		i[2].mi.time = 0;
		i[2].mi.dwExtraInfo = 0;
		i[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
		i[3].type = INPUT_MOUSE;
		i[3].mi.dx = old.x;
		i[3].mi.dy = old.y;
		i[3].mi.mouseData = 0;
		i[3].mi.time = 0;
		i[3].mi.dwExtraInfo = 0;
		i[3].mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
		SendInput(4, i, sizeof(INPUT));
		ShowCursor(TRUE);
	}
};

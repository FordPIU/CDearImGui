#include "CImGui.h"

#include <iostream>

using namespace std;

int main() {
	COpenGL cOpen = COpenGL();
	CWindow cWindow = CWindow("CDearImGui", 800, 800);
	CImGui cGui = CImGui(cWindow, "Resource A");

	while (!cWindow.shouldClose())
	{
		cOpen.newFrame();
		cGui.newFrame();
		cGui.text("Hello World!");
		cGui.text("This is a 2nd line!");
		cGui.endFrame();
		cWindow.endFrame();
	}

	cGui.endProccess();
	cWindow.endProccess();

	return 0;
}
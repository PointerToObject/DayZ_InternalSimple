// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"



DWORD WINAPI Hack(HMODULE hModule) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	bool bPlayerList = false, bBreakLoop = false;
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(DayZ);
	while (true) {
		uint64_t* ptr2world = (uint64_t*)(moduleBase + World);
		int* ntc = (int*)(*ptr2world + NearTableCount);

		if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
			bPlayerList = !bPlayerList;
		}
		if (bBreakLoop) {
			break;
		}
		if (bPlayerList) {
			while (true) {
				for (int i = 0; i < *ntc; i++) {
					uint64_t* ntPtr = (uint64_t*)(*ptr2world + NearTable);
					uint64_t* entity = (uint64_t*)(*ntPtr + (i * 0x8));
					uint64_t* visStatePtr = (uint64_t*)(*entity + 0x1D0);
					float* xpos = (float*)(*visStatePtr + 0x2C);
					float* ypos = (float*)(*visStatePtr + 0x30);
					float* zpos = (float*)(*visStatePtr + 0x34);

					printf("[%d] Entity at - [ X:%f | Y:%f | Z:%f ]\n", i, *xpos, *ypos, *zpos);
				}
				Sleep(10);
				system("CLS");
			}
		}
		else {
			printf("[off] playerlist\n");
		}
		printf("Entity Count %d\n", *ntc);
		Sleep(10);
		system("CLS");
	}
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD fwReason,
	LPVOID reserve
) {
	switch (fwReason) {
	case DLL_PROCESS_ATTACH:
	{
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Hack, hModule, 0, nullptr));
	}
	case DLL_PROCESS_DETACH:
	{
		break;
	}
	}
	return 1;
}




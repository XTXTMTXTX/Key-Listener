#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<windows.h>
#include<winuser.h>
#include"fmod/inc/fmod.h"
#include"fmod/inc/fmod_errors.h"
using namespace std;
FMOD_SYSTEM *fmodSystem;
FMOD_SOUND *KeySound[0xFF];
FMOD_SOUND *MouseSound[5];
bool KS[0xFF];
bool Key_Enabled[0xFF];
bool Mouse_Enabled[5];
bool UseKey = 1, UseMouse = 1;
bool UseKeys[0xFF];
DWORD g_main_tid=0;
HHOOK g_kb_hook=0;
HHOOK g_m_hook=0;

inline int readNUM() {
	int x=0,f=1;
	char ch=getchar();
	while(ch<'0'||ch>'9') {
		if(ch=='-')f=-1;
		ch=getchar();
	}
	while(ch>='0'&&ch<='9') {
		x=x*10+ch-'0';
		ch=getchar();
	}
	return x*f;
}
inline void Sound_Key(const int &key) {
	if(!UseKeys[key])return;
	if(Key_Enabled[key])FMOD_System_PlaySound(fmodSystem, KeySound[key], 0, false, 0);else
	if(Key_Enabled[0])FMOD_System_PlaySound(fmodSystem, KeySound[0], 0, false, 0);
	FMOD_System_Update(fmodSystem);
}
inline void Sound_Mouse(const int &key) {
	if(Mouse_Enabled[key])FMOD_System_PlaySound(fmodSystem, MouseSound[key], 0, false, 0);else
	if(Mouse_Enabled[0])FMOD_System_PlaySound(fmodSystem, MouseSound[0], 0, false, 0);
	FMOD_System_Update(fmodSystem);
}

BOOL CALLBACK con_handler(DWORD) {
	PostThreadMessage(g_main_tid,WM_QUIT,0,0);
	return TRUE;
};
LRESULT CALLBACK kb_proc(int code,WPARAM w,LPARAM l) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)l;
	if((w == WM_KEYDOWN||w == WM_SYSKEYDOWN)&&(!KS[(p->vkCode)])) {
		KS[(p->vkCode)] = 1;
		Sound_Key(p->vkCode);
	}
	if(w == WM_KEYUP||w == WM_SYSKEYUP)KS[(p->vkCode)] = 0;
	return CallNextHookEx(g_kb_hook, code, w, l);
}
LRESULT CALLBACK m_proc(int code,WPARAM w,LPARAM l) {
    MOUSEHOOKSTRUCT * p = (MOUSEHOOKSTRUCT *)l;
    if(p != NULL){
    	switch (w){
    		case WM_LBUTTONDOWN:
    			Sound_Mouse(1);
    			break;
    		case WM_RBUTTONDOWN:
    			Sound_Mouse(2);
    			break;
    		case WM_MBUTTONDOWN:
    			Sound_Mouse(3);
    			break;
    		case WM_XBUTTONDOWN:
    			Sound_Mouse(4);
    			break;
    	}
    }
	
    return CallNextHookEx(g_m_hook, code, w, l);
}
int main(int argc, char* argv[]) {
	printf("FMOD Studio Low Level API (C) Firelight Technologies Pty Ltd.\n");
	printf("For custom key bindings, refer keycodes on site:\n");
	printf("    https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes\n");

	FMOD_RESULT initRet = FMOD_System_Create(&fmodSystem);
	if (initRet != FMOD_OK) {
		printf("Create FMOD System Failed: %s\n", FMOD_ErrorString((FMOD_RESULT)initRet));
		return initRet;
	}

	if (argc >= 2) {
		int b = atoi(argv[1]);
		if (b == 0) b = 128;
		FMOD_System_SetDSPBufferSize(fmodSystem, b, 2);
	} else FMOD_System_SetDSPBufferSize(fmodSystem, 128, 2);

	FMOD_System_SetOutput(fmodSystem, FMOD_OUTPUTTYPE_ASIO);
	int driverId, driverNums;
	FMOD_System_GetNumDrivers(fmodSystem, &driverNums);
	char name[256];
	int systemRate;
	int speakerChannels;

	for (int i = 0; i < driverNums; i++) {
		FMOD_System_GetDriverInfo(fmodSystem, i, name, 255, 0, &systemRate, 0, &speakerChannels);
		printf("DeviceID: %-3d DeviceName: %s  Rate: %d  Channels: %d\n", i, name, systemRate, speakerChannels);
	}
	printf("Please select the DeviceID: ");
	driverId = readNUM();
	FMOD_System_SetDriver(fmodSystem, driverId);
	FMOD_System_GetDriver(fmodSystem, &driverId);
	FMOD_System_GetDriverInfo(fmodSystem, driverId, name, 255, 0, &systemRate, 0, &speakerChannels);
	unsigned bufLen;
	int bufNum;
	FMOD_System_GetDSPBufferSize(fmodSystem, &bufLen, &bufNum);

	initRet = FMOD_System_Init(fmodSystem, 32, FMOD_INIT_NORMAL, 0);
	if (initRet != FMOD_OK) {
		printf("FMOD System Initialize Failed: %s\n", FMOD_ErrorString((FMOD_RESULT)initRet));
		return initRet;
	}
	printf("FMOD System Initialize Finished.\n");
	printf("[FMOD] Device Name: %s\n", name);
	printf("[FMOD] Device Sample Rate: %d\n", systemRate);
	printf("[FMOD] Device Channels: %d\n", speakerChannels);
	printf("[FMOD] DSP buffer size: %d * %d\n", bufLen, bufNum);
	printf("[FMOD] Latency: %.10lfms\n", bufLen * bufNum * 1000.0 / systemRate);

	for (int i=0; i<0xFF; i++) {
		char path[20];
		if(i==0)sprintf(path,"Key_Default.wav");else sprintf(path,"Key_%d.wav",i);
		if (FMOD_OK == FMOD_System_CreateSound(fmodSystem, path, FMOD_UNIQUE, 0, &KeySound[i])) {
			printf("[FMOD] Loaded Sample (%s)\n", path);
			Key_Enabled[i] = 1;
		}
	}
	for (int i=0; i<5; i++) {
		char path[20];
		switch (i){
			case 0:
				sprintf(path,"Click_Default.wav");
				break;
			case 1:
				sprintf(path,"Click_L.wav");
				break;
			case 2:
				sprintf(path,"Click_R.wav");
				break;
			case 3:
				sprintf(path,"Click_M.wav");
				break;
			case 4:
				sprintf(path,"Click_X.wav");
				break;
		}
		if (FMOD_OK == FMOD_System_CreateSound(fmodSystem, path, FMOD_UNIQUE, 0, &MouseSound[i])) {
			printf("[FMOD] Loaded Sample (%s)\n", path);
			Mouse_Enabled[i] = 1;
		}
	}
	if (argc >= 3) {
		int b = atoi(argv[2]);
		if(b)UseKey=1;else UseKey=0;
	}
	if (argc >= 4) {
		int b = atoi(argv[3]);
		if(b)UseMouse=1;else UseMouse=0;
	}
	if(argc > 4 && UseKey){
		for(int i=4; i<argc; i++){
			int b = atoi(argv[i]);
			if(b>=0xFF || b<1)continue;
			UseKeys[b] = 1;
		}
	}else if(UseKey)memset(UseKeys, 1, sizeof(UseKeys));
	g_main_tid = GetCurrentThreadId();
	SetConsoleCtrlHandler(&con_handler, 1);
	if(UseKey)g_kb_hook = SetWindowsHookEx(WH_KEYBOARD_LL, &kb_proc, GetModuleHandle(NULL),0);
	if(UseMouse)g_m_hook = SetWindowsHookEx(WH_MOUSE_LL, &m_proc, GetModuleHandle(NULL),0);
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if(UseKey)UnhookWindowsHookEx(g_kb_hook);
	if(UseMouse)UnhookWindowsHookEx(g_m_hook);
	
	if (initRet == FMOD_OK && fmodSystem != nullptr) {
		FMOD_System_Release(fmodSystem);
		printf("FMOD System released.\n");
	}

	return 0;
}

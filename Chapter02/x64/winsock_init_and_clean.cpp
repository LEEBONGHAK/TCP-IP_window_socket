#pragma comment(lib, "ws2_32")
#include <winsock2.h>

int main(int argc, char *argv[])
{
	// 원속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, L"원속 초기화 완료", L"알림", MB_OK);

	// 윈속 종료
	WSACleanup();
	return 0;
}
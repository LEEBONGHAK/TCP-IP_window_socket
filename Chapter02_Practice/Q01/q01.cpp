#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	cout << "wsa.wVersion: " << wsa.wVersion << endl;
	cout << "wsa.wHighVersion: " << wsa.wHighVersion << endl;
	cout << "wsa.szDescription: " << wsa.szDescription << endl;
	cout << "wsa.szSystemStatus: " << wsa.szSystemStatus << endl;

	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	// 윈속 종료
	WSACleanup();
	return 0;
}
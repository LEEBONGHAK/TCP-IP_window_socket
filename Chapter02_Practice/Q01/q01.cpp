#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

/*
	Q01
	
	첫 번째 InitWinsock 예제를 수정하여 wsa 변수의 필드 중 처음 네 개(wVersion, wHighVersion, szDescription, szSystemStatus)를 첫 화면에 출력하시요.
	윈속 1.1 버전을 사용하도록 예제를 수정할 경우 이 값들이 어떻게 바뀌는가?
*/

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
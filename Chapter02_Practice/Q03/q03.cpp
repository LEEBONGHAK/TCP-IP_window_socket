#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>

/*
	Q03

	두 번째 InitWinsock 예제를 수정하여 IPv6 프로토콜에 기반한 TCP 소켓을 생성하시오.
*/

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int main(int argc, char** argv)
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	// socket();
	SOCKET tcp_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET)
		err_quit("socket()");

	// closesocket()
	closesocket(tcp_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50 // 50바이트 버퍼를 사용해 데이터를 보낸다.

// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_dispaly(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char **argv)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// connect() 호출에 사용할 변수
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	if (inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr) != 1)
		err_quit("inet_pton()");
	serveraddr.sin_port = htons(SERVERPORT);

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE]; // BUFSIZE 크기인 버퍼를 선언
	const char *testdata[] = {
		// 전송할 문자열 데이터 선언
		"안녕하세요",
		"반가워요",
		"오늘따라 할 이야기가 많을 것 같네요",
		"저도 그렇네요",
	};
	int len; // 문자열 길이를 담을 변수 선언

	// 서버와 통신
	for (int i = 0; i < 4; i++)
	{
		// socket()
		// 데이터를 보낼 때마다 소켓 생성
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
			err_quit("socket()");

		// connect()
		// 데이터를 보낼 때마다 서버 접속
		retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR)
			err_quit("connect()");

		// 데이터 입력(시뮬레이션)
		len = strlen(testdata[i]);
		strncpy_s(buf, sizeof(buf), testdata[i], len); // 문자열 데이터를 버퍼에 복사

		// 데이터 보내기
		// 문자열 데이터를 가변 길이로 보낸다.
		retval = recv(sock, buf, len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_dispaly("send()");
			break;
		}

		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// closesocket()
		// 데이터를 보낼 때마다 접속 종료
		closesocket(sock);
	}

	// 윈속 종료
	WSACleanup();
	return 0;
}
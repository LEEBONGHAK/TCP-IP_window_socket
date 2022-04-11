#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP	"127.0.0.1"
#define SERVERPORT	9000
#define BUFSIZE		50

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR) lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL
	);
	printf("[%s] %s\n", msg, (char *) lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char **argv)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");

	// connet()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_ntoa(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connet(sock, (SOCKADDR *) &serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("connet()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE];	// BUFSIZE 크기인 버퍼 선언
	char *testdata[] = {	// 전송할 문자열 데이터 선언
		"안녕하세요",
		"반가워요",
		"오늘따라 할 이야기가 많을 것 같네요",
		"저도 그렇네요",
	};
	int len;	// 문자열 길이를 계산한 결과를 받는다.

	// 서버과 데이터 통신
	for (int i = 0; i < 4; i++)
	{
		// 데이터 입력(시뮬레이션)
		len = strlen(testdata[i]);
		strncpy(buf, testdata[i], len);	// 문자열 데이터를 버퍼에 복사

		// 데이터 보내기(고정 길이)
		// 문자열 길이는 32비트(int) 고정 길이로 보낸다.
		retval = send(sock, (char *) &len, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		// 데이터 보내기(가변 길이)
		// 문자열 데이터를 가변 길이로 보낸다.
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		printf("[TCP 클라이언트] %d+%d바이트를 보냈습니다.\n", sizeof(int), retval);
	}
	
	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
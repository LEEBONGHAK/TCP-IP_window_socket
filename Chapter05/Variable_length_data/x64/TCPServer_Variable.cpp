#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 512 // 실제로 받을 데이터는 이보다 작은 가변길이 데이터라고 가정

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
void err_display(const char *msg)
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

// 내부 구현용 함수
// 소켓 수신 버퍼에서 데이터를 한 번에 많이 읽은 후 1바이트씩 리턴해주는 사용자 정의 함수
int _recv_ahead(SOCKET s, char *p)
{
	// 소켓 수신 버퍼에서 읽은 데이터를 한 바이트씩 리턴하는 데 필요한 '핵심 변수'
	// 함수가 리턴해도 값을 계속 유지해야 하기 때문에 'static'으로 선언
	// __declspec(thread): 멀티 스레트 환경에서도 함수가 문제없이 동작하는데 필요
	__declspec(thread) static int nbytes = 0;
	__declspec(thread) static char buf[1024];
	__declspec(thread) static char *ptr;

	// 소켓 수신 버퍼에서 읽어들인 데이터가 아직 없거나 리턴하여 모두 소모한 경우 새로 읽어 'buf[]'에 저장해두고 표인터 변수 'ptr'이 맨 앞쪽 바이트를 가리키게 한다.
	if (nbytes == 0 || nbytes == SOCKET_ERROR)
	{
		nbytes = recv(s, buf, sizeof(buf), 0);
		if (nbytes == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (nbytes == 0)
			return 0;

		ptr = buf;
	}

	// 남은 바이트 수('nbytes')를 1 감소시키고, 포인터 변수 ptr이 가리키는 데이터를 포인터 변수 'p'가 가리키는 메모리 영역에 넣어 리턴
	--nbytes;
	*p = *ptr++;
	return 1;
}

// 사용자 정의 데이터 수신 함수 - '\n'이 나올 때까지 데이터를 읽기 위해 정의
// 소켓 's'에서 데이터를 한 바이트씩 읽어 'buf'가 가리키는 메모리 영역에 저장하되, '\n'이 나오거나 최대 길이(maxlen - 1)에 도달하면 '\0'을 붙여 리턴한다. '\0'는 반드시 필요하지는 않지만 데이터를 문자열로 간주해 출력할 때 유용하다.
int recvline(SOCKET s, char *buf, int maxlen)
{
	int n, nbytes;
	char c, *ptr = buf;

	for (n = 1; n < maxlen; n++)
	{
		nbytes = _recv_ahead(s, &c);
		if (nbytes == 1)
		{
			*ptr++ = c;
			if (c == '\n')
				break;
		}
		else if (nbytes == 0)
		{
			*ptr = 0;
			return (n - 1);
		}
		else
			return SOCKET_ERROR;
	}

	*ptr = 0;
	return n;
}

int main(int argc, char **argv)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1]; // null 문자를 감안

	// ip address buf
	char ipaddr[512];

	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		inet_ntop(AF_INET, &clientaddr.sin_addr, ipaddr, sizeof(ipaddr));
		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", ipaddr, ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		while (1)
		{
			// 데이터 받기
			// 'recvline()' 함수를 호출해 '\n'이 나올 때까지 데이터를 받는다. '+ 1'은 1바이트 null 문자를 감안한 것으로 실제로 최대 BUFSIZE(=512) 크기만큼 데이터를 읽는다.
			retval = recvline(client_sock, buf, BUFSIZE + 1);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// 받은 데이터 출력
			// 'buf[]' 배열에 있는 데이터 자체에 '\n\0'가 포함되어 있어 줄바꿈 없이 화면에 문자열로 출력하면 된다.
			printf("[TCP/%s:%d] %s", ipaddr, ntohs(clientaddr.sin_port), buf);
		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", ipaddr, ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
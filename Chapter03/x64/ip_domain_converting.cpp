#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define TESTNAME	"www.example.com"
#define BUFSIZE		512

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("도메인 이름(변환 전) = %s\n", TESTNAME);

	ADDRINFO hint;
	ZeroMemory(&hint, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;

	// 도메인 이름 -> IPv4
	ADDRINFO *name;
	int errcode, errcode2;
	if (!(errcode = getaddrinfo(TESTNAME, "443", &hint, &name)))
	{
		// 성공이면 결과 출력
		char buf[BUFSIZE + 1];
		inet_ntop(AF_INET, name->ai_addr, buf, BUFSIZE);
		int buflen = strlen(buf);
		buf[buflen] = '\0';
		printf("IP 주소(변환 후) = %s\n", buf);

		// IP 주소 -> 도메인 이름
		SOCKADDR_IN domainaddr;
		domainaddr.sin_family = AF_INET;
		inet_pton(AF_INET, buf, &domainaddr);
		domainaddr.sin_port = 443;
		char buf2[BUFSIZE + 1];
		ZeroMemory(buf, sizeof(buf));
		ZeroMemory(buf2, sizeof(buf2));

		if (!(errcode2 = getnameinfo((SOCKADDR*)&domainaddr, sizeof(domainaddr), buf, BUFSIZE, buf2, BUFSIZE, NI_NUMERICHOST)))
		{
			buflen = strlen(buf);
			int buflen2 = strlen(buf2);
			buf[buflen] = '\0';
			buf2[buflen2] = '\0';
			// 성공이면 결과 출력
			printf("도메인 이름(다시 변환 후) [NODE] = %s\n", buf);
			printf("도메인 이름(다시 변환 후) [SERVICE] = %s\n", buf2);
		}
	}
	freeaddrinfo(hint);
	freeaddrinfo(name);
	
	// getaddrinfo() 에러 발생 시 에러 코드와 에러 메세지 출력
	if (errcode)
		printf("[errcode] error code: %d, message: %s\n", errcode, gai_strerror(errcode));

	// getnameinfo() 에러 발생 시 에러코드와 에러 메세지 출력
	if (errcode2)
		printf("[errorcode2] error code: %d, message: %s\n", errcode2, gai_strerror(errcode2));

	WSACleanup();
	return 0;
}

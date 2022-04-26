#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT	9000
#define BUFSIZE		512

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)&lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

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

int main(int argc, char** argv)
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");

	SOCKADDR_IN6 serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_addr = in6addr_any;
	serveraddr.sin6_port = htons(SERVERPORT);
	retval = bind(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	SOCKADDR_IN6 peeraddr;
	char buf[BUFSIZE + 1];
	int addrlen;

	while (1)
	{
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR*)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR)
		{
			err_display("recvfrom()");
			continue;
		}

		char ipaddr[50];
		inet_ntop(AF_INET6, &peeraddr.sin6_addr, ipaddr, sizeof(ipaddr));

		buf[retval] = '\0';
		printf("[UDP/%s: %d] %s\n", ipaddr, ntohs(peeraddr.sin6_port), buf);

		retval = sendto(sock, buf, retval, 0, (SOCKADDR*)&peeraddr, addrlen);
		if (retval == SOCKET_ERROR)
		{
			err_display("sendto()");
			continue;
		}
	}

	closesocket(sock);

	WSACleanup();
	return 0;
}

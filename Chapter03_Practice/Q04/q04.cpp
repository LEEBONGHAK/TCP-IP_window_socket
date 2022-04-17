#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#define TESTNAME "www.ieft.org"

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

BOOL GetIPAddr6(char *name, SOCKADDR_IN6 **result)
{
    ADDRINFO hints;
    ADDRINFO *tmp;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(name, "443", &hints, &tmp) == SOCKET_ERROR)
    {
        err_display("getaddrinfo()");
        return FALSE;
    }

    *result = (SOCKADDR_IN6 *)tmp->ai_addr;

    freeaddrinfo(tmp);
    return TRUE;
}

int main(int argc, char **argv)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    printf("도메인 이름(변환 전) = %s\n", TESTNAME);

    SOCKADDR_IN6 *result;
    char ipaddr[512];
    DWORD ipaddrlen = sizeof(ipaddr);

    if (GetIPAddr6((char *)TESTNAME, &result))
    {
        WSAAddressToString((SOCKADDR *)result, sizeof(*result), NULL, ipaddr, &ipaddrlen);
        printf("IP 주소(변환 후) = %s\n", ipaddr);
    }

    WSACleanup();
    return 0;
}
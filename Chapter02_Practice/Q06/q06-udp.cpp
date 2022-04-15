#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

/*
    Q06

    socket() 함수와 기본 역할은 같지만 좀 더 향상된 기능을 제공하는 WSASocket() 함수가 있다.
    WSASocket() 함수를 사용해 TCP 소켓과 UDP 소켓을 생성하시오.
*/

// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)&lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

int main(int argc, char **argv)
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;
    MessageBox(NULL, "윈속 초기화 완료", "알림", MB_OK);

    // socket()
    SOCKET udp_sock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, 0);
    if (udp_sock == INVALID_SOCKET)
        err_quit("socket()");

    // closesocket()
    closesocket(udp_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
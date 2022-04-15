#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

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

int main(int argc, char **argv)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    u_short x1 = 0x1234;
    u_long y1 = 0x12345678;
    u_short x2;
    u_long y2;
    u_short x3;
    u_long y3;

    // socket()
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        err_quit("socket()");

    // 호스트 바이트 -> 네트워크 바이트
    printf("[호스트 바이트 -> 네트워크 바이트]\n");
    if (WSAHtons(sock, x1, &x2) == SOCKET_ERROR)
        err_quit("WSAHtons()");
    printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));

    if (WSAHtonl(sock, y1, &y2) == SOCKET_ERROR)
        err_quit("WSAHtonl()");
    printf("0x%x -> 0x%x\n", y1, y2);

    // 네트워크 바이트 -> 호스트 바이트
    printf("[네트워크 바이트 -> 호스트 바이트]\n");
    if (WSANtohs(sock, x2, &x3) == SOCKET_ERROR)
        err_quit("WSANtohs()");
    printf("0x%x -> 0x%x\n", x2, x3);

    if (WSANtohl(sock, y2, &y3) == SOCKET_ERROR)
        err_quit("WSANtohl()");
    printf("0x%x -> 0x%x\n", y2, y3);

    // 잘못된 사용 예
    u_long wrong;
    printf("\n[잘못된 사용 예]\n");
    if (WSAHtonl(sock, x1, &wrong) == SOCKET_ERROR)
        err_quit("WSAHtonl()");
    printf("0x%x -> 0x%x\n", x1, wrong);

    // closesocket()
    closesocket(sock);

    WSACleanup();
    return 0;
}
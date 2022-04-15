#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

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

int main(int argc, char **argv)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    for (int i = 1; i < argc; i++)
    {
        HOSTENT *ptr = gethostbyname(argv[i]);
        if (ptr == NULL)
        {
            err_display("gethostbyname()");
            break;
        }

        printf("[alias names]\n");
        for (int j = 0; ptr->h_aliases[j]; j++)
            printf("%s\n", ptr->h_addr_list[j]);
        printf("\n");

        printf("[addresses]\n");
        for (int j = 0; ptr->h_addr_list[j]; j++)
            printf("%s\n", inet_ntoa(*(IN_ADDR *)ptr->h_addr_list[j]));
        printf("\n");
    }

    WSACleanup();
    return 0;
}
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <malloc.h>

#define SERVERPORT  11021
#define BUFSIZE     512

#define endl '\n'
using namespace std;

struct PacketData
{
    WCHAR DataSize;
    WCHAR PacketID;
    char Type;
    char *BodyData;
};

// 오류 출력 후 종료
void err_quit(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL
    );
    MessageBox(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// 오류 출력
void err_display(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL
    );
    wcout << "[" << msg << "] " << (LPCTSTR)lpMsgBuf << endl;
    LocalFree(lpMsgBuf);
}

// 패킷을 free해주는 함수
void free_packet(PacketData* packet)
{
    free(packet->BodyData);
    free(packet);
}

// 받아온 정보를 페킷 구조체에 저장하는 함수
PacketData *get_packet_info(char* buf)
{
    char* tmp = buf;
    char* data;
    PacketData *packet;

    packet = (PacketData*)calloc(1, sizeof(PacketData));
    if (packet == NULL)
        return NULL;

    memcpy(&packet->DataSize, tmp, sizeof(packet->DataSize));
    tmp += sizeof(packet->DataSize);

    memcpy(&packet->PacketID, tmp, sizeof(packet->PacketID));
    tmp += sizeof(packet->PacketID);

    memcpy(&packet->Type, tmp, sizeof(packet->Type));
    tmp += sizeof(packet->Type);

    data = (char *)calloc((int) packet->DataSize - 5, sizeof(char));
    if (data == NULL)
    {
        free(packet);
        return NULL;
    }
    memcpy(data, tmp, (int)packet->DataSize - 5);
    packet->BodyData = data;

    return packet;
}

// 전송할 버퍼를 설정하는 함수
void set_buf(char *buf, int result)
{
    WCHAR DataSize = (WCHAR)(sizeof(int) + 5);
    WCHAR PacketID = (WCHAR)31;
    char Type = 0;

    memcpy(buf, &DataSize, sizeof(WCHAR));
    buf += sizeof(WCHAR);
    memcpy(buf, &PacketID, sizeof(WCHAR));
    buf += sizeof(WCHAR);
    memcpy(buf, &Type, sizeof(char));
    buf += sizeof(char);
    memcpy(buf, &result, sizeof(result));
}

// 계산하는 함수
int make_result(char* data, WCHAR flag)
{
    int result = -1;
    int num[3] = { 0, };
    WCHAR opt[2];

    if (flag == (WCHAR)21)
    {
        memcpy(&num[0], data, sizeof(int));
        data += sizeof(int);
        memcpy(&opt[0], data, sizeof(WCHAR));
        data += sizeof(WCHAR);
        memcpy(&num[1], data, sizeof(int));

        if (opt[0] == (WCHAR)0)
            result = num[0] + num[1];
        else if (opt[0] == (WCHAR)1)
            result = num[0] - num[1];
        else if (opt[0] == (WCHAR)2)
            result = num[0] * num[1];
        else if (opt[0] == (WCHAR)3)
            result = num[0] % num[1];
    }
    else
    {
        memcpy(&num[0], data, sizeof(int));
        data += sizeof(int);
        memcpy(&opt[0], data, sizeof(WCHAR));
        data += sizeof(WCHAR);
        memcpy(&num[1], data, sizeof(int));
        data += sizeof(int);
        memcpy(&opt[1], data, sizeof(WCHAR));
        data += sizeof(WCHAR);
        memcpy(&num[2], data, sizeof(int));

        if (opt[0] == (WCHAR)0 && opt[1] == (WCHAR)0)
            result = num[0] + num[1] + num[2];
        else if (opt[0] == (WCHAR)0 && opt[1] == (WCHAR)1)
            result = num[0] + num[1] - num[2];
        else if (opt[0] == (WCHAR)0 && opt[1] == (WCHAR)2)
            result = num[0] + num[1] * num[2];
        else if (opt[0] == (WCHAR)0 && opt[1] == (WCHAR)3)
            result = num[0] + num[1] % num[2];
        else if (opt[0] == (WCHAR)1 && opt[1] == (WCHAR)0)
            result = num[0] - num[1] + num[2];
        else if (opt[0] == (WCHAR)1 && opt[1] == (WCHAR)1)
            result = num[0] - num[1] - num[2];
        else if (opt[0] == (WCHAR)1 && opt[1] == (WCHAR)2)
            result = num[0] - num[1] * num[2];
        else if (opt[0] == (WCHAR)1 && opt[1] == (WCHAR)3)
            result = num[0] - num[1] % num[2];
        else if (opt[0] == (WCHAR)2 && opt[1] == (WCHAR)0)
            result = num[0] * num[1] + num[2];
        else if (opt[0] == (WCHAR)2 && opt[1] == (WCHAR)1)
            result = num[0] * num[1] - num[2];
        else if (opt[0] == (WCHAR)2 && opt[1] == (WCHAR)2)
            result = num[0] * num[1] * num[2];
        else if (opt[0] == (WCHAR)2 && opt[1] == (WCHAR)3)
            result = num[0] * num[1] % num[2];
        else if (opt[0] == (WCHAR)3 && opt[1] == (WCHAR)0)
            result = num[0] % num[1] + num[2];
        else if (opt[0] == (WCHAR)3 && opt[1] == (WCHAR)1)
            result = num[0] % num[1] - num[2];
        else if (opt[0] == (WCHAR)3 && opt[1] == (WCHAR)2)
            result = num[0] % num[1] * num[2];
        else if (opt[0] == (WCHAR)3 && opt[1] == (WCHAR)3)
            result = num[0] % num[1] % num[2];
    }
    
    return result;
}

int main()
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_sock == INVALID_SOCKET)
        err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
        err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
        err_quit("listen()");
    
    // 데이터 통신에서 사용할 변수
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE];
    PacketData* packet_received;
    int result;

    // ip address buf
    char ipaddr[BUFSIZE];

    while (1)
    {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            err_display("accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        inet_ntop(AF_INET, &clientaddr.sin_addr, ipaddr, sizeof(ipaddr));
        cout << "[TCP 서버] 클라이언트 접속: IP 주소=" << ipaddr << ", 포트 번호=" << ntohs(clientaddr.sin_port) << endl;

        // 클라이언트와 데이터 통신
        while (1)
        {
            ZeroMemory(buf, BUFSIZE + 1);

            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR)
            {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            packet_received = get_packet_info(buf);
            if (packet_received == NULL)
                continue;

            if (packet_received->PacketID == (WCHAR)21 || packet_received->PacketID == (WCHAR)22)
                result = make_result(packet_received->BodyData, packet_received->PacketID);
            else
            {
                free_packet(packet_received);
                continue;
            }

            if (result == -1)
            {
                free_packet(packet_received);
                continue;
            }

            ZeroMemory(buf, BUFSIZE);
            set_buf(buf, result);

            retval = send(client_sock, buf, 9, 0);
            if (retval == SOCKET_ERROR)
            {
                free_packet(packet_received);
                err_display("send()");
                break;
            }

            // 전송한 바이트 수 출력
            wcout << "[TCP/" << ipaddr << ":" << ntohs(clientaddr.sin_port) << "] " << retval << "바이트를 보냈습니다." << endl;

            free_packet(packet_received);
        }

        // closesocket()
        closesocket(client_sock);
        cout << "[TCP 서버] 클라이언트 종료: IP 주소=" << ipaddr << ", 포트 번호=" << ntohs(clientaddr.sin_port) << endl;
    }

    // closesocket()
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();

    return 0;
}

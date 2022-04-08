#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	// 원속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	/* IPv4 변환 연습 */

	// 원래 IPv4 주소 출력
	const char *ipv4test = "147.46.114.70";
	printf("IPv4 주소(변환 전) = %s\n\n", ipv4test);

	// inet_pton() 함수 연습
	IN_ADDR ipv4num;
	inet_pton(AF_INET, ipv4test, &(ipv4num.S_un));
	printf("IPv4 주소(변환 후) = 0x%x\n", ipv4num.S_un);

	// WSAStringToAddressW() 함수 언습
	SOCKADDR_IN ipv4num2;
	int addrlen = sizeof(ipv4num2);
	WSAStringToAddressW((LPWSTR)L"147.46.114.70", AF_INET, NULL, (SOCKADDR *)&ipv4num2, &addrlen);
	printf("WSAStingToAddressW(): IPv4 주소(변환 후) = 0x%x\n\n", ipv4num2.sin_addr);

	// inet_ntop() 함수 연습
	char buf[512];
	inet_ntop(AF_INET, &(ipv4num.S_un), buf, sizeof(buf));
	int bufsize = strlen(buf);
	buf[bufsize] = '\0';
	printf("IPv4 주소(다시 변환 후) = %s\n", buf);

	// WSAAddressToStringW() 함수 연습
	char buf2[512];
	DWORD buf2size = sizeof(buf);
	WSAAddressToStringW((SOCKADDR *)&ipv4num2, sizeof(ipv4num2), NULL, (LPWSTR)buf2, &buf2size);
	printf("WSAAddressToStringW(): IPv6 주소(다시 변환 후) = %s\n\n\n", buf2);

	/* IPv6 변환 연습 */

	// 원래 IPv6 주소 출력
	const char *ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	printf("IPv6 주소(변환 전) = %s\n\n", ipv6test);

	// WSAStringToAddressW() 함수 언습
	SOCKADDR_IN6 ipv6num;
	int addrlen2 = sizeof(ipv6num);
	WSAStringToAddressW((LPWSTR)L"2001:0230:abcd:ffab:0023:eb00:ffff:1111", AF_INET6, NULL, (SOCKADDR *)&ipv6num, &addrlen2);
	printf("WSAStingToAddressW(): IPv6 주소(변환 후) = 0x");
	for (int i = 0; i < 16; i++)
		printf("%02x", ipv6num.sin6_addr.u.Byte[i]);
	printf("\n");

	// inet_pton() 함수 연습
	IN6_ADDR ipv6num2;
	inet_pton(AF_INET6, ipv6test, &(ipv6num2.u));
	printf("inet_pton(): IPv6 주소(변환 후) = 0x");
	for (int i = 0; i < 16; i++)
		printf("%02x", ipv6num2.u.Byte[i]);
	printf("\n\n");

	// WSAAddressToStringW() 함수 연습
	char ipaddr[512];
	DWORD ipaddrlen = sizeof(ipaddr);
	WSAAddressToStringW((SOCKADDR *)&ipv6num, sizeof(ipv6num), NULL, (LPWSTR)ipaddr, &ipaddrlen);
	printf("WSAAddressToStringW(): IPv6 주소(다시 변환 후) = %s\n", ipaddr);

	// inet_ntop() 함수 연습
	char ipaddr2[512];
	inet_ntop(AF_INET6, &(ipv6num2.u), ipaddr2, sizeof(ipaddr2));
	int ipaddrsize = strlen(ipaddr2);
	ipaddr2[ipaddrsize] = '\0';
	printf("inet_ntop(): IPv6 주소(다시 변환 후) = %s\n", ipaddr2);
	printf("\n");

	// 윈속 종료
	WSACleanup();
	return 0;
}
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	/* IPv4 변환 연습 */

	// 원래 IPv4 주소 출력
	char *ipv4test = "123.456.678.0";
	printf("IPv4 주소(변환 전) = %s\n", ipv4test);

	// inet_addr() 함수 연습
	printf("IPv4 주소(변환 후) = 0x%x\n", inet_addr(ipv4test));

	// inet_ntoa() 함수 연습
	IN_ADDR ipv4num;
	ipv4test.s_addr = inet_addr(ipv4test);
	printf("IPv4 주소(다시 변환 후) = %s\n", inet_ntoa(ipv4num));
	printf("\n");


	/* IPv6 변환 연습 */

	// 원래 IPv6 주소 출력
	char *ipv6test = "0000:1234:abcd:ffab:0023:eb00:ffff:1111";
	printf("IPv6 주소(변환 전) = %s\n", ipv6test);

	// WSAStringToAddress() 함수 언습
	SOCKADDR_IN6 ipv6num;
	int addrlen = sizeof(ipv6test);
	WSAStringToAddress(ipv6test, AF_INET6, NULL, (SOCKADDR *) &ipv6num, &addrlen);
	printf("IPv6 주소(변환 후) = 0x");
	for (int i = 0; i < 16; i++)
		printf("%02x", ipv6num.sin6_addr.u.Byte[i]);
	printf("\n");

	// WSAAddressToString() 함수 연습
	char ipaddr[50];
	DWORD ipaddrlen = sizeof(ipaddr);
	WSAAddressToString((SOCKADDR *) &ipv6num, sizeof(ipv6num), NULL, &ipaddr, &ipaddrlen);
	printf("IPv6 주소(다시 변환 후) = %s\n", ipaddr);

	WSACleanup();
	return 0;
}
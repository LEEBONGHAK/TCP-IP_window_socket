# Q04

본문에서 정의한 `GetIPAddr()` 함수와 비슷한 역할을 하는 `GetIPAddr6()` 함수를 정의하시오.  
단, 내부적으로는 `getaddrinfo()` 함수를 사용함으로써 도메인 이름에 대응하는 IPv6 주소를 얻어야 한다.  
테스트할 때는 'www.example.com', 'www.ietf.org', 'ipv6.google.com' 중 하나를 사용한다.  
  
> IPv6이 아닌 IPv4만 얻어지는 버그 발생 중 수정해야함 또한 `GetIPAddr6()` 함수의 인수가 기존 함수의 인수와 다른 문제점도 존재
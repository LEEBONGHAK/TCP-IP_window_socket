# Q05

사용자 정의 함수도 일반 소켓 함수와 같은 방법으로 소켓 오류를 처리하도록 만들 수 있다.  
다음 내용을 참고하여 f() 함수를 정의하시오.(힌트: WSASetLastError())  
  
<함수 정의>  
```
// x가 0 또는 양수면 성공. 리턴값은 0이고, 오류 코드는 0으로 설정한다.
// x가 음수면 실패. 리턴값은 SOCKET_ERROR고, 오류 코드는 WSEAMSGSIZE로 설정한다.
int f(int x)
{
    ...
}
```
  
<함수 사용법>  
```
int retval = f(...);
if (retval == SOCKET_ERROR)
    err_quit("f()");
```
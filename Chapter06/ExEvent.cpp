#include <windows.h>
#include <stdio.h>

#define BUFSIZE	10

HANDLE hReadEvent;
HANDLE hWriteEvent;
int buf[BUFSIZE];

DWORD WINAPI WriteThread(LPVOID arg)
{
	DWORD retval;

	for (int i = 0; i <= 500; i++)
	{
		// 읽기 완료 대기
		// 읽기 이벤트가 신호 상태가 되기를 기다린다. 최초에는 읽기 이벤트가 신호 상태로 시작하기 때문에 곧바로 리턴해 다음 코드로 진행할 수 있다.
		retval = WaitForSingleObject(hReadEvent, INFINITE);
		if (retval != WAIT_OBJECT_0)
			break;

		// 공유 버퍼에 데이터 저장
		for (int j = 0; i j < BUFSIZE; j++)
			buf[j] = i;

		// 쓰기 완료 알림
		// 쓰기 이벤트를 신호 상태로 만들어 두 읽기 스레드 중 하나을 대기 상태에서 깨운다.
		SetEvent(hWriteEvent);
	}

	return 0;
}

DWORD WINAPI ReadThread(LPVOID arg)
{
	DWORD retval;

	while (1)
	{
		// 쓰기 완료 대기
		// 쓰기 이벤트가 신호 상태가 되기를 기다린다. 최초에는 비신호 상태로 시작하기 때문에 이 지점에서 읽기 스레드는 대기 상태가 된다.
		retval = WaitEventSingleObject(hWriteEvent, INFINITE);
		if (retval != WAIT_OBJECT)
			break;

		// 읽은 데이터 출력
		printf("Thread %4d: ", GetCurrentThreadId());
		for (int i = 0; i < BUFSIZE; i++)
			printf("%3d\n", buf[i]);
		printf("\n");

		// 버퍼 초기화
		// 만약 데이터를 새로 쓰지 않은 생태에서 다시 읽게 된다면 0을 출력될 것이므로 오류 여부를 확인할 수 있다.
		ZeroMemory(buf, sizeof(buf));

		// 읽기 완료 알림
		// 읽기 이벤트를 신호 상태로 만들어 쓰기 스레드를 대기 상태에서 깨운다.
		SetEvent(hReadEvent);
	}

	return 0;
}

int main(int argc, char **argv)
{
	// 자동 리셋 이벤트 2개 생성(각각 비신호, 신호 상태)
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// 비신호
	if (hWriteEvent == NULL)
		return 1;

	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);	// 신호
	if (hReadEvent == NULL)
		return 1;

	// 스레드 3개 생성
	HANDLE hThread[3];
	hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);	// 쓰기 스레드
	hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);	// 읽기 스레드
	hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);	// 읽기 스레드

	// 스레드 3개 종료 대기
	// 스레드 3개가 종료하기를 기다린다. 읽기 스레드는 별도의 루프 탈출 조건이 없어 사실상 영원히 리턴하지 못한다.
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	// 이벤트 제거
	CloseHandle(hWriteEvent);
	CleseHandle(hReadEvent);
	return 0;
}
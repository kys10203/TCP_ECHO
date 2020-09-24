#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

enum Color
{
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY, 
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
};

void textcolor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ShowErrorMessage(string message)
{
	cout << "Error occured : " << message << endl;
	system("pause");
	exit(1);
}

int main(void)
{
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9935;
	char received[256];
	string sent;

	textcolor(LIGHTBLUE, BLACK);

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// Winsock 초기화
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);		// TCP 소켓 생성
	if(clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 문자열IP를네트워크바이트형식으로
	serverAddress.sin_port = htons(serverPort); // 2바이트정수네트워크바이트형식으로

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "connect() complete !" << endl;

	while (1) { // 반복적으로서버로메시지를전송하고에코메시지를전달받습니다.
		textcolor(BLACK, WHITE);
		cout << "[to server] ";
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);

		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';

		if (strcmp(received, "exit") == 0) {
			textcolor(LIGHTBLUE, BLACK);
			cout << "=====서버 종료=====" << endl;
			break;
		}
		textcolor(BLACK, DARKGRAY);
		cout << "[from server] " << received << endl;
	}
	closesocket(clientSocket);
	WSACleanup();
	system("pause");

	return 0;
}
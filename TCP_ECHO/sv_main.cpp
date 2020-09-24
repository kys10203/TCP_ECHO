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
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9935;
	char received[256];

	textcolor(LIGHTBLUE, BLACK);

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// Winsok 초기화
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// TCP 소켓 생성
	
	if(serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(serverPort);

	if(bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "bind() complete !" << endl;

	if(listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "listen() complete !" << endl;

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);

	if(clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	cout << "accept() complete !" << endl;

	while(1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
			received[length] = NULL;

			textcolor(BLACK, WHITE);
			cout << "[from client] " << received << endl;
			textcolor(BLACK, DARKGRAY);
			cout << "[to client] " << received << endl;
			if(strcmp(received, "exit") == 0) {
				send(clientSocket, received, sizeof(received) -1, 0);
				textcolor(LIGHTBLUE, BLACK);
				cout << "=====서버 종료=====" << endl;
				break;
			}
			send(clientSocket, received, sizeof(received) -1, 0);
	}
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");

	return 0;
}
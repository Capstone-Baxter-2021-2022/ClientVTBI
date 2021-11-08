#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "127.0.0.1";	//changed	"10.17.30.107"	//IP Address of the server
	int port = 54000;						//Listening port # on the server

	//Initialized WinSock

	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start WinSock, Err #" << wsResult << endl;
		return;
	}

	//Create Socket

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't crete socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
	}

	//Fill in a hint structure

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//Connect to server

	int connResult = connect(sock, (sockaddr*)& hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	//send and receive data
	char buf[4096];
	string VTBI;
	
	do {
		ifstream inFile;
		//open the file stream
		inFile.open("F:\\Capstone\\GUI_Revisions\\GUI_Revisions\\eztest\\Test.txt");
		//Read from the stream
		if (inFile.is_open()) {
			while (getline(inFile, VTBI)) { //read data from file object and put it into string.
				int sendResult = send(sock, VTBI.c_str(), VTBI.size() + 1, 0); //send data
				if (sendResult != SOCKET_ERROR)
				{
					//Wait for response
					ZeroMemory(buf, 4096);
					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						//Echo response to console
						cout << "SERVER>" << string(buf, 0, bytesReceived) << endl;
					}
				}

			}
			inFile.close(); //close the file object.
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	} while (VTBI.size() > 0);

	//Close down everything
	closesocket(sock);
	WSACleanup();
}

#include"Headers.h"
#include"Client.h"

int main(void) 
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	Client client("127.0.0.1", 30104);
	if (!client.InitWinsock()) return 1;
	if (!client.CreateSocket()) return 1;
	if (!client.ConnectToServer()) return 1;
	client.Run();

	return 0;
}
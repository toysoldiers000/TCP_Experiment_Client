#include "TCPClient.h"
#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include <tchar.h>
#include <QDebug>
#include <QByteArray>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <qtextcodec.h>


SOCKET servsock;
char buffer[1024] = "\0";

TCPClient::TCPClient(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.sendButton,&QPushButton::clicked,this,&TCPClient::sendButton);
	connect(ui.quitButton, &QPushButton::clicked, this, &TCPClient::stopButton);

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)//初始化 ws2_32.dll 动态库
	{
		qDebug()<<"WSAStartup() failed!\n";//Winsock 初始化错误
		return ;
	}
	if (wsaData.wVersion != wVersionRequested)
	{
		qDebug() << "The version of Winsock is not suited!\n";//Winsock 版本不匹配
		WSACleanup();//结束对 ws2_32.dll 的调用
		return ;
	}
	//创建套接字
	
	qDebug() << "Create Socket...\n";
	servsock = socket(AF_INET, SOCK_STREAM, 0);
	int servport = 5555;
	int iSockErr = 0;
	//定义服务器地址结构
	sockaddr_in tcpaddr; memset(&tcpaddr, 0, sizeof(tcpaddr));
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(servport);
	//将一个点分十进制 IP 地址字符串转换成 32 位数字表示的 IP 地址
	tcpaddr.sin_addr.s_addr = inet_addr("10.8.46.182");//根据 TCP 服务器的 IP 地址提示
	// 连接服务器
	iSockErr = ::connect(servsock, (sockaddr*)&tcpaddr, sizeof(tcpaddr));
	if (iSockErr == SOCKET_ERROR)
	{
		qDebug() << "connect unsccessful!\n";
		iSockErr = closesocket(servsock);
		if (iSockErr == SOCKET_ERROR)
			qDebug() << "closesocket unsccessful!\n";
		return ;
	}
	int len = 0;

}



char* TCPClient::RC4(char* C, char* key)
{
	int S[256];
	int T[256];

	int  count = 0;
	count = strlen(key);

	for (int i = 0; i < 256; i++)
	{
		S[i] = i;
		int tmp = i % count;
		T[i] = key[tmp];
	}

	int j = 0;

	for (int i = 0; i < 256; i++)
	{
		j = (j + S[i] + T[i]) % 256;
		int tmp;
		tmp = S[j];
		S[j] = S[i];
		S[i] = tmp;
	}

	int length = 0;
	length = strlen(C);

	int i;
	i = 0, j = 0;

	for (int p = 0; p < length; p++)
	{

		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		int tmp;
		tmp = S[j];
		S[j] = S[i];
		S[i] = tmp;

		int k = S[(S[i] + S[j]) % 256];
		C[p] = C[p] ^ k;
	}

	return C;
}

void TCPClient::sendButton()
{
	QString message;
	message = ui.clear->toPlainText();
	qDebug() << message;

	

	memset(buffer, 0, sizeof(buffer));

	QTextCodec *tc = QTextCodec::codecForName("UTF-8");
	QByteArray qs2ch = message.toLocal8Bit();

	sprintf_s(buffer, qs2ch.data());
	RC4(buffer, key);
	ui.cipher->setPlainText(QString(QLatin1String(buffer)));
	send(servsock, buffer, sizeof(buffer), 0);
}

void TCPClient::stopButton()
{
	close();
}
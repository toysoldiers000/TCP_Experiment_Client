#pragma once

#include <QtWidgets/QWidget>
#include "ui_TCPClient.h"

class TCPClient : public QWidget
{
	Q_OBJECT

public:
	TCPClient(QWidget *parent = Q_NULLPTR);
	char* RC4(char* C, char* key);

public slots:
	void sendButton();
	void stopButton();

private:
	Ui::TCPClientClass ui;
	char key[256] = "password";
};

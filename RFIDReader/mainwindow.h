#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QString>
#include "Device.h"
#include "console.h"

QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


signals:
	void EndThread(void);

public slots:
	void PrintTags(QString);
	void UpdateStatus(QString);

private slots :
	void about();
	

private:
    void initActionsConnections();
	void ConnectDevice();
	void DisconnectedDevice();
	void MenuDisconnectDevice();
	void UiDisconnect();


private:
	Ui::MainWindow *ui;
    Console *console;
	QLabel *errorLabel;
	QLabel *statusLabel;
	Device* thread;
};

#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "Device.h"

#include <QMessageBox>
#include <QLabel>
#include <QString>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
	, thread(new Device(this))
	, errorLabel(new QLabel(this))
	, statusLabel(new QLabel(this))
{
    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);
	setCentralWidget(console); 

	errorLabel->setText("Error: ");
	statusBar()->addWidget(statusLabel);
	statusBar()->addWidget(errorLabel);

    ui->actionConnect_Device->setEnabled(true);
	ui->actionDisconnect_Device->setEnabled(false);
    initActionsConnections();

}

MainWindow::~MainWindow()
{
    delete ui;
	delete thread;
	delete statusLabel;
	delete errorLabel;
	delete console;
}

void MainWindow::initActionsConnections()
{
	connect(ui->menuExit, &QMenu::aboutToShow, this, &MainWindow::close);
	connect(ui->actionConnect_Device, &QAction::triggered, this, &MainWindow::ConnectDevice);
	connect(ui->actionDisconnect_Device, &QAction::triggered, this, &MainWindow::MenuDisconnectDevice);
	connect(thread, &Device::FatalError, this, &MainWindow::DisconnectedDevice);
	connect(thread, &Device::isFinished, this, &MainWindow::DisconnectedDevice);
	connect(thread, &Device::PassError, this, &MainWindow::UpdateStatus);
	connect(thread, &Device::PassTags, this, &MainWindow::PrintTags);
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("Information"),
                       tr("This is an interface to print tags read by an RFID reader"));
}

void MainWindow::ConnectDevice()
{
	ui->actionConnect_Device->setEnabled(false);
	ui->actionDisconnect_Device->setEnabled(true);
	statusLabel->setText("CONNECTED");
	thread->start();
}

void MainWindow::DisconnectedDevice()
{
	UiDisconnect();
}

void MainWindow::MenuDisconnectDevice()
{
	UiDisconnect();
	emit EndThread();
}

void MainWindow::UiDisconnect()
{
	ui->actionConnect_Device->setEnabled(true);
	ui->actionDisconnect_Device->setEnabled(false);
	statusLabel->setText("DISCONNECTED");
}

void MainWindow::UpdateStatus(QString str)
{ 
	errorLabel->setText("Error: " + str);
}

void MainWindow::PrintTags(QString tag)
{
	console->putString(tag);
}

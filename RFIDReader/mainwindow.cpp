#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "Device.h"

#include <QMessageBox>
#include <QLabel>
#include <QString>
#include <QStatusBar>


/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: MainWindow.cpp - A simple RFID reader designed to read from SkyeTek RFID readers. 
--
-- PROGRAM: RFIDReader.exe
--
-- FUNCTIONS:
-- void MainWindow::initActionsConnections()
-- void MainWindow::about()
-- void MainWindow::ConnectDevice()
-- void MainWindow::DisconnectedDevice()
-- void MainWindow::MenuDisconnectDevice()
-- void MainWindow::UiDisconnect()
-- void MainWindow::UpdateStatus()
--
-- DATE: October 15th 2017
--
-- REVISIONS: minimal product completed October 13th (reading tags in debug)
-- Final version completed October 17th 2017
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- NOTES:
-- 
-- Will run without an RFID reader plugged in, however it will fail when you attempt to connect to a device.
-- You can connect and disconnect as many times as you wish during a session. 
-- This application prints out a list of all tags that has been scanned by the application since it was launched.
-- As it scans new tags, it will persistently display previous tags to the screen. 
----------------------------------------------------------------------------------------------------------------------*/
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

	statusBar()->addWidget(statusLabel);
	statusBar()->addWidget(errorLabel);

    ui->actionConnect_Device->setEnabled(true);
	ui->actionDisconnect_Device->setEnabled(false);
    initActionsConnections();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~MainWindow
--
-- DATE: October 13th 2017
--
-- REVISIONS: 1
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot
--
-- INTERFACE: MainWindow::~MainWindow
--
-- RETURNS: N/A
--
-- NOTES:
-- Cleans up allocated memory when exiting the UI.
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
	delete thread;
	delete statusLabel;
	delete errorLabel;
	delete console;
}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: initActionsConnections
--
-- DATE: October 15th 2017
--
-- REVISIONS: 1
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot
--
-- INTERFACE: void MainWindow::initActionsConnections()
--
--
-- RETURNS: void
--
-- NOTES:
-- Intializes connections between singals and slots - essentially mapping Qt events. 
-- Accesses UI and Device thread to link to MainWindow Functions
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::initActionsConnections()
{
	connect(ui->menuExit, &QMenu::aboutToShow, this, &MainWindow::close);
	connect(ui->actionConnect_Device, &QAction::triggered, this, &MainWindow::ConnectDevice);
	connect(ui->actionDisconnect_Device, &QAction::triggered, this, &MainWindow::MenuDisconnectDevice);
	connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
	connect(thread, &Device::FatalError, this, &MainWindow::DisconnectedDevice);
	connect(thread, &Device::isFinished, this, &MainWindow::DisconnectedDevice);
	connect(thread, &Device::PassError, this, &MainWindow::UpdateStatus);
	connect(thread, &Device::PassTags, this, &MainWindow::PrintTags);
}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: about
--
-- DATE: October 15th 2017
--
-- REVISIONS:
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot
--
-- INTERFACE: void MainWindow::about()
--
-- RETURNS: void
--
-- NOTES:
-- Shows QMessageBox with brief info about project.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::about()
{
    QMessageBox::about(this, tr("Information"),
                       tr("This is an interface to print tags read by an RFID reader"));
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ConnectDevice
--
-- DATE: October 15th 2017
--
-- REVISIONS: 
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- INTERFACE: void MainWindow::ConnectDevice()
-- 
--
-- RETURNS: void
-- 
-- NOTES:
-- ConnectDevice starts the Device thread and updates the UI to indicate that it is connected.
-- If the connection fails, the disconnection is handled by the Device thread.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::ConnectDevice()
{
	ui->actionConnect_Device->setEnabled(false);
	ui->actionDisconnect_Device->setEnabled(true);
	statusLabel->setText("CONNECTED");
	thread->start();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: DisconnectedDevice
--
-- DATE: October 15th 2017
--
-- REVISIONS: 
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- INTERFACE: void MainWindow::DisconnectedDevice()
--
-- RETURNS: void
-- 
-- NOTES:
-- Called when the Device thread ends itself because of a fatal error. Is signaled by a FatalError event.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::DisconnectedDevice()
{
	UiDisconnect();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MenuDisconnectDevice
--
-- DATE: October 15th 2017
--
-- REVISIONS: 
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- INTERFACE: void MainWindow::MenuDisconnectDevice
--
-- RETURNS: void
-- 
-- NOTES:
-- This function is signaled by clicking the disconnect menu item. Updates the UI and signals the read thread by emiting 
-- an EndThread event. 
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::MenuDisconnectDevice()
{
	UiDisconnect();
	emit EndThread();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UiDisconnect
--
-- DATE: October 15th 2017
--
-- REVISIONS: 1
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot
--
-- INTERFACE: void MainWindow::UiDisconnect()
-- 
--
-- RETURNS: void
-- 
-- NOTES:
-- Updates the UI when the reader is disconnected. Sets status bar label to "DISCONNECTED"
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::UiDisconnect()
{
	ui->actionConnect_Device->setEnabled(true);
	ui->actionDisconnect_Device->setEnabled(false);
	statusLabel->setText("DISCONNECTED");
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UpdateStatus
--
-- DATE: October 15th 2017
--
-- REVISIONS: 1
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- INTERFACE: void MainWindow::UpdateStatus()
--
-- RETURNS: void
--
-- NOTES:
-- Updates status text when an error occurs based on the type of error. 
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::UpdateStatus(QString str)
{ 
	errorLabel->setText("Error: " + str);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: PrintTags
--
-- DATE: October 15th 2017
--
-- REVISIONS: 1
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- INTERFACE: void MainWindow::PrintTags()
--
-- RETURNS: void
--
-- NOTES:
-- Sends a single tag to the console putTag function. That function in turns adds the tag to the console's list of tags. 
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::PrintTags(QString tag)
{
	console->putTag(tag);
}

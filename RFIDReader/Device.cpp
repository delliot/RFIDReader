#pragma once

#include "mainwindow.h"
#include <QDebug>
#include <QSet>


/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Device.cpp - A simple RFID reader designed to read from SkyeTek RFID readers.
--
-- PROGRAM: RFIDReader.exe
--
-- FUNCTIONS:
-- void Device::StopReading()
-- void Device::run()
-- QString Device::toQString(TCHAR const * str)
--
-- DATE: October 13th 2017
--
-- REVISIONS: October 15th 2017
--			  Improved Error handling.
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- NOTES:
-- Device is a QThread which, when created, attemps to conenct to an RFID reader. If it successful, it begins reading
-- tags and sending those tags back to the MainWindow. If it unsuccessful, the thread frees its memory and exits back to the 
-- UI thread, throwing a FatalError object back to MainWindow. 
----------------------------------------------------------------------------------------------------------------------*/
Device::Device(QObject *parent)
	: QThread(parent)
	, mNumDevices(0)
	, mNumReaders(0)
	, mDevices(NULL)
	, mReaders(NULL)
{
}

Device::~Device()
{
	SkyeTek_FreeDevices(mDevices, mNumDevices);
	SkyeTek_FreeReaders(mReaders, mNumReaders);
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
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- INTERFACE: void Device::StopReading()
--
-- RETURNS: void
--
-- NOTES:
-- This slot is signal when the "Disconnect" menu item is selected in MainWindow. ends the read loop in run().
----------------------------------------------------------------------------------------------------------------------*/
void Device::StopReading()
{
	this->isReading = false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: run
--
-- DATE: October 15th 2017
--
-- REVISIONS: October 13th
--			  Initial read loop implemented
--			  October 15th
--			  Improved reading and error handling
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- INTERFACE: void Device::run()
--
-- RETURNS: void
--
-- NOTES:
-- RFID reading happens here. Checks for connected readers, and if a reader is connected, then continues to a read loop
-- where it continues to scan for tags until the user selects "Disconnect" in the UI.
----------------------------------------------------------------------------------------------------------------------*/
void Device::run()
{
	LPSKYETEK_TAG* tags = NULL;
	SKYETEK_STATUS status;
	this->isReading = true;
	mNumDevices = 0;
	mNumReaders = 0;

	mNumDevices = SkyeTek_DiscoverDevices(&mDevices);
	if (mNumDevices == 0)
	{
		SkyeTek_FreeReaders(mReaders, mNumReaders);
		SkyeTek_FreeDevices(mDevices, mNumDevices);
		emit PassError("no readers");
		emit FatalError();
		this->isReading = false;
		exit();
	}

	mNumReaders = SkyeTek_DiscoverReaders(mDevices, mNumDevices, &mReaders);
	if (mNumReaders == 0)
	{
		SkyeTek_FreeReaders(mReaders, mNumReaders);
		SkyeTek_FreeDevices(mDevices, mNumDevices);
		emit FatalError();
		this->isReading = false;
		exit();
	}
	
	while (isReading)
	{
		unsigned short numTags = 0;
		status = SkyeTek_GetTags(mReaders[0], AUTO_DETECT, &tags, &numTags);
		QString tmp;
		
		for (unsigned short j = 0; j < numTags; j++)
		{
			emit PassTags(toQString(tags[j]->friendly));
		}
		SkyeTek_FreeTags(mReaders[0], tags, numTags);
	}

	SkyeTek_FreeReaders(mReaders, mNumReaders);
	SkyeTek_FreeDevices(mDevices, mNumDevices);
	exit();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: toQString
--
-- DATE: October 15th 2017
--
-- REVISIONS: N/A
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot
--
-- INTERFACE: QString Device::toQString(TCHAR const * str)
--
-- RETURNS: a QString, which can be used with all Qt functions.
--
-- NOTES:
-- The SkyeTek API returns a TCHAR * when the tags are fetched from the reader, and it needs to be converted into a 
-- useable wrapper.
----------------------------------------------------------------------------------------------------------------------*/
QString Device::toQString(TCHAR const * str)
{
	QString tmp = QString("");
	for (unsigned int i = 0; str[i] != '\0'; i++)
	{
		tmp.append((LPSTR)(str + i));
	}
	return tmp;
}


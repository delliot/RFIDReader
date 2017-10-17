#pragma once

#include "mainwindow.h"
#include <QDebug>
#include <QSet>


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

void Device::StopReading()
{
	this->isReading = false;
}

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

QString Device::toQString(TCHAR const * str)
{
	QString tmp = QString("");

	for (unsigned int i = 0; str[i] != '\0'; i++)
	{
		tmp.append((LPSTR)(str + i));
	}

	return tmp;
}


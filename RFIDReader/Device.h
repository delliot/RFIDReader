#ifndef API_H
#define API_H
#include <stdio.h>
#include <SkyeTekAPI.h>
#include <SkyeTekProtocol.h>
#endif

#ifndef DEVICE_H
#define DEVICE_H

#include <QThread>
#include <QObject>
#include <QString>

class Device : public QThread
{
	Q_OBJECT

public:
	Device(QObject *parent);
	~Device();
	QString toQString(TCHAR const * str);
	virtual void run();

signals:
	void PassTags(QString str);
	void PassError(QString str);
	void FatalError();

public slots:
	void StopReading();

private:
	LPSKYETEK_DEVICE * mDevices;
	LPSKYETEK_READER * mReaders;
	BOOL isReading;
	int mNumDevices;
	int mNumReaders;
	
};

#endif


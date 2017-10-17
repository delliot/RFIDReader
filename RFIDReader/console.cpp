#include "console.h"
#include "mainwindow.h"


Console::Console(QWidget *parent)
	: QPlainTextEdit(parent)
{
	setOfTags = new QSet<QString>();
}

Console::~Console()
{
	delete setOfTags;
}

void Console::putData(const QByteArray &data)
{
	insertPlainText(QString(data));
}

void Console::putString(const QString &str)
{
	setOfTags->insert(str);
	Console::PrintAllTags();
}

void Console::PrintAllTags()
{
	QSetIterator<QString> i(*setOfTags);
	clear();
	while (i.hasNext())
	{
		insertPlainText(QString("Tag: ") + i.next() + QString("\n"));
	}
}



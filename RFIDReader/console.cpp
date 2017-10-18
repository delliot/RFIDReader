#include "console.h"
#include "mainwindow.h"


/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: console.cpp - A simple RFID reader designed to read from SkyeTek RFID readers.
--
-- PROGRAM: RFIDReader.exe
--
-- FUNCTIONS:
-- void Console::~Console()
-- void Console::putTag(const QString &str)
-- void Console::PrintAllTags()
--
-- DATE: October 15th 2017
--
-- REVISIONS: N/A
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- NOTES:
-- Keeps a QSet of tags that have been scanned in the current session and displays them to the screen.
-- Console inherits from QPlainTextEdit, a Qt display widget. 
----------------------------------------------------------------------------------------------------------------------*/
Console::Console(QWidget *parent)
	: QPlainTextEdit(parent)
{
	setOfTags = new QSet<QString>();
}

Console::~Console()
{
	delete setOfTags;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: putTag
--
-- DATE: October 15th 2017
--
-- REVISIONS: 1
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- INTERFACE: void Console::putTag(const QString &str)
--				const QString &str: tag to add to set of tags		
--
-- RETURNS: void
--
-- NOTES:
-- Adds tag to the set of tags, if the tag is not already in the set. Then PrintAllTags is called to repaint all tags to 
-- the Console.
----------------------------------------------------------------------------------------------------------------------*/
void Console::putTag(const QString &str)
{
	setOfTags->insert(str);
	Console::PrintAllTags();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: PrintAllTags
--
-- DATE: October 15th 2017
--
-- REVISIONS: 1
--
-- DESIGNER: Delan Elliot/Juliana French
--
-- PROGRAMMER: Delan Elliot/Juliana French
--
-- INTERFACE: void Console::PrintAllTags()
--
-- RETURNS: void
--
-- NOTES:
-- Clears Console, the iterates through the set of tags printing them to the console.
----------------------------------------------------------------------------------------------------------------------*/
void Console::PrintAllTags()
{
	QSetIterator<QString> i(*setOfTags);
	clear();
	while (i.hasNext())
	{
		insertPlainText(QString("Tag: ") + i.next() + QString("\n"));
	}
}



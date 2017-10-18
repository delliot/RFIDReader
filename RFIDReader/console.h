#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

class Console : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void getData(const QByteArray &data);
    void exitData();

public:
    explicit Console(QWidget *parent = 0);
	virtual ~Console();
	void putTag(const QString &str);
private:
	QSet<QString>* setOfTags;
	void PrintAllTags();
};

#endif

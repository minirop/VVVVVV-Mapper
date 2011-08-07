#ifndef VVVVVVHANDLER_H
#define VVVVVVHANDLER_H

#include <iostream>
#include <QXmlDefaultHandler>
#include <QXmlParseException>
#include <QXmlAttributes>
#include <QStringList>
#include <QString>
#include <QHash>

class VVVVVVHandler : public QXmlDefaultHandler
{
public:
	VVVVVVHandler();
	
	bool startElement(const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts);
	bool endElement(const QString & namespaceURI, const QString & localName, const QString & qName);
	bool error(const QXmlParseException & exception);
	bool fatalError(const QXmlParseException & exception);
	bool warning (const QXmlParseException & exception);
	bool characters(const QString & ch);
	
	void saveTo(const QString & filename);

private:
	QString currentTag;
	QXmlAttributes currentAttributes;
	
	int mapwidth;
	int mapheight;
	
	QList<int> chunks;
};

#endif

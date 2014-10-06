#ifndef VVVVVVHANDLER_H
#define VVVVVVHANDLER_H

#include <QXmlDefaultHandler>
#include <QXmlParseException>
#include <QXmlAttributes>
#include <QStringList>
#include <QString>
#include <QList>
#include <QPair>
#include <QRect>
#include <QPoint>
#include <QPixmap>
#include <QBitmap>

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
	
	QPixmap spriteset;
	
	int mapwidth;
	int mapheight;
	int mapwidthScreen;
	int mapheightScreen;
	
	QList<int> chunks;
	QList< QPair<QPoint, QPixmap > > sprites;
	QList< QPair<QPoint, QPixmap> > pixmaps;
	QList< QPair<QPoint, QColor> > crew;
	QList< QPair<QPoint, QBitmap> > whiteObjets;
	QList< QPoint > plainColoredObjects;
	
	QVector< QMap<QString, int> > screenTilesets;
	
	QBitmap character, checkdown, checkup, trinket;
	QVector<QPoint> colorPositionFromId[2];
};

#endif

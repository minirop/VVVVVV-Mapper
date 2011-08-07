#include "VVVVVVHandler.h"
#include <QPixmap>
#include <QPainter>
#include <QDebug>

VVVVVVHandler::VVVVVVHandler()
{
}

bool VVVVVVHandler::startElement(const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts)
{
	currentTag = localName;
	currentAttributes = atts;
	
	return true;
}

bool VVVVVVHandler::characters(const QString & ch)
{
	if(currentTag == "mapwidth")
	{
		mapwidth = ch.toInt()*40;
	}
	else if(currentTag == "mapheight")
	{
		mapheight = ch.toInt()*30;
	}
	else if(currentTag == "edentity")
	{
		int type = currentAttributes.value("t").toInt();
		int x = currentAttributes.value("x").toInt();
		int y = currentAttributes.value("y").toInt();
		int p1 = currentAttributes.value("p1").toInt();
		int p2 = currentAttributes.value("p2").toInt();
		int p3 = currentAttributes.value("p3").toInt();
		
		switch(type)
		{
		case 16: // captain/start
			for(int x1 = x;x1 < x+2;x1++)
			{
				for(int y1 = y;y1 < y+3;y1++)
				{
					//chunks[y1*mapwidth+x1] = 10000;
				}
			}
			break;
		case 15: // crew members
			for(int x1 = x;x1 < x+2;x1++)
			{
				for(int y1 = y;y1 < y+3;y1++)
				{
					//chunks[y1*mapwidth+x1] = 10001;
				}
			}
			break;
		case 9: // trinket
			for(int x1 = x;x1 < x+2;x1++)
			{
				for(int y1 = y;y1 < y+2;y1++)
				{
					//chunks[y1*mapwidth+x1] = 10002;
				}
			}
			break;
		case 11: // grav line
			// p1 = is vertical ?
			// p2 = first line or column
			// p3 = length in pixel (blocs * 8)
			p3 = p3/8;
			if(p1)
			{
				for(int yy = p1;yy < p1+p3;yy++)
				{
					//chunks[yy*mapwidth+x] = 10003;
				}
			}
			else
			{
				for(int xx = p1;xx < p1+p3;xx++)
				{
					//chunks[y*mapwidth+xx] = 10003;
				}
			}
			break;
		}
	}
	else if(currentTag == "contents")
	{
		QStringList strchunks = ch.split(",");
		foreach(QString ch, strchunks)
		{
			chunks.push_back(ch.toInt());
		}
	}
	
	return true;
}

void VVVVVVHandler::saveTo(const QString & filename)
{
	QPixmap tileset("tiles.png");
	QPixmap image(mapwidth*8, mapheight*8);
	image.fill(Qt::black);
	
	QPainter p;
	p.begin(&image);
	
	for(int y = 0;y < mapheight;y++)
	{
		for(int x = 0;x < mapwidth;x++)
		{
			int ch = chunks[x+y*mapwidth];
			int xt = ch%40;
			int yt = ch/40;
			p.drawPixmap(x*8, y*8, tileset.copy(xt*8, yt*8, 8, 8));
		}
	}
	
	p.end();
	
	image.save(filename);
}

bool VVVVVVHandler::endElement(const QString & namespaceURI, const QString & localName, const QString & qName)
{
	currentTag = QString();
	return true;
}

bool VVVVVVHandler::error(const QXmlParseException & exception)
{
	return false;
}

bool VVVVVVHandler::fatalError(const QXmlParseException & exception)
{
	return false;
}

bool VVVVVVHandler::warning (const QXmlParseException & exception)
{
	return false;
}

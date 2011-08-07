#include "VVVVVVHandler.h"
#include <QPainter>
#include <QDebug>

VVVVVVHandler::VVVVVVHandler()
{
	QPixmap spriteset("sprites.png");
	// crew
	QPixmap pix = spriteset.copy(QRect(7, 2, 10, 21));
	character = pix.mask();
	// checkpoints
	pix = spriteset.copy(QRect(288, 32, 16, 16));
	checkdown = pix.mask();
	pix = spriteset.copy(QRect(256, 32, 16, 16));
	checkup = pix.mask();
	// trinket
	pix = spriteset.copy(QRect(320, 32, 16, 16));
	trinket = pix.mask();
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
		
		QPoint point;
		QRect rect;
		
		switch(type)
		{
		case 1: // enemies
			break;
		case 2: // moving and conveyor
			// p1 =
			// - 0 moving down
			// - 1 moving up
			// - 2 moving left
			// - 3 moving right
			// - 4 unknown/unused/motionless
			// - 5 short right conveyor
			// - 6 short left conveyor
			// - 7 long right conveyor
			// - 8 long left conveyor
			break;
		case 3: // disappear
			{
				QPixmap pix(32, 8);
				pix.fill();
				
				point = QPoint(x*8, y*8);
				pixmaps.push_back(qMakePair(point, pix));
			}
			break;
		case 9: // trinket
			{
				QBitmap bit(trinket);
				point = QPoint(x*8, y*8);
				whiteObjets.push_back(qMakePair(point, bit));
			}
			break;
		case 10: // checkpoint
			// p1 = is upside down ? 1 = no, 0 = yes
			{
				QBitmap bit(checkup);
				if(p1)
					bit = checkdown;
				
				point = QPoint(x*8, y*8);
				whiteObjets.push_back(qMakePair(point, bit));
			}
			break;
		case 11: // grav line
			// p1 = is vertical ?
			// p2 = first line or column
			// p3 = length in pixel (blocs * 8)
			if(p1)
			{
				QPixmap line(2, p3);
				line.fill();
				
				point = QPoint(x*8+3, (y/30)*240 + p2*8);
				pixmaps.push_back(qMakePair(point, line));
			}
			else
			{
				QPixmap line(p3, 2);
				line.fill();
				
				point = QPoint((x/40)*320 + p2*8, y*8+3);
				pixmaps.push_back(qMakePair(point, line));
			}
			break;
		case 13: // mini-teleporters
			break;
		case 15: // crew members
			// p1 = 0: cyan / pink / yellow / red / green / 5: blue
			{
				QColor color;
				switch(p1)
				{
					case 0:
						color = QColor(0, 255, 255);
						break;
					case 1:
						color = QColor(255, 0, 255);
						break;
					case 2:
						color = QColor(255, 255, 0);
						break;
					case 3:
						color = QColor(255, 0, 0);
						break;
					case 4:
						color = QColor(0, 255, 0);
						break;
					case 5:
					default:
						color = QColor(0, 0, 255);
				}
				
				point = QPoint(x*8-1, y*8+3);
				crew.push_back(qMakePair(point, color));
			}
			break;
		case 16: // captain/start
			// p1 = is looking left ? 1 = <<, 0 = >>
			if(p1)
				rect = QRect(103, 2, 10, 21);
			else
				rect = QRect(7, 2, 10, 21);
			
			point = QPoint(x*8-1, y*8+3);
			sprites.push_back(qMakePair(point, rect));
			break;
		case 17: // text string
			break;
		case 18: // terminal
			break;
		case 19: // script box
			break;
		case 50: // wrap line
			break;
		default:
			qDebug() << type << x << y;
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
	QPixmap spriteset("sprites.png");
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
	
	QListIterator< QPair< QPoint, QRect > > i(sprites);
	while(i.hasNext())
	{
		QPair<QPoint, QRect> pair = i.next();
		p.drawPixmap(pair.first, spriteset.copy(pair.second));
	}
	
	QListIterator< QPair< QPoint, QPixmap > > ip(pixmaps);
	while(ip.hasNext())
	{
		QPair<QPoint, QPixmap> pair = ip.next();
		p.drawPixmap(pair.first, pair.second);
	}
	
	QListIterator< QPair< QPoint, QColor > > ic(crew);
	while(ic.hasNext())
	{
		QPair<QPoint, QColor> pair = ic.next();
		p.setPen(pair.second);
		p.drawPixmap(pair.first, character);
	}
	
	QListIterator< QPair< QPoint, QBitmap > > iw(whiteObjets);
	while(iw.hasNext())
	{
		QPair<QPoint, QBitmap> pair = iw.next();
		p.setPen(Qt::white);
		p.drawPixmap(pair.first, pair.second);
	}
	
	p.end();
	
	image.save(filename);
	
	qDebug() << "finished";
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

#include "VVVVVVHandler.h"
#include <QPainter>
#include <QDebug>

const int SCREEN_WIDTH = 40;
const int SCREEN_HEIGHT = 30;
const int MAX_SCREEN_WIDTH = 20;

VVVVVVHandler::VVVVVVHandler() : spriteset("sprites.png")
{
	// crew
	QPixmap pix = spriteset.copy(QRect(4, 31, 16, 24));
	character = pix.mask();
	
	// checkpoints
	pix = spriteset.copy(QRect(288, 32, 16, 16));
	checkdown = pix.mask();
	pix = spriteset.copy(QRect(256, 32, 16, 16));
	checkup = pix.mask();
	
	// terminals
	pix = spriteset.copy(QRect(128, 24, 16, 24));
	terminaldown = pix.mask();
	pix = spriteset.copy(QRect(160, 24, 16, 24));
	terminalup = pix.mask();
	
	// trinket
	pix = spriteset.copy(QRect(320, 32, 16, 16));
	trinket = pix.mask();
	
	// teleports
	pix = spriteset.copy(QRect(192, 32, 16, 16));
	teleportbig = pix.mask();
	pix = spriteset.copy(QRect(224, 32, 16, 16));
	teleportsmall = pix.mask();
	
	// colors
	for(int i = 0;i < 32;i++)
	{
		int x = (((i % 11 + 1) * 3) + 1) * 8;
		int y = ((i / 11 + 1) * 5) * 8;
		colorPositionFromId.push_back(QPoint(x, y));
	}
	
	colorFromId[1].push_back(QColor("#39568c"));
	colorFromId[1].push_back(QColor("#9c2a2a"));
	colorFromId[1].push_back(QColor("#2a9c9b"));
	colorFromId[1].push_back(QColor("#7d24a2"));
	colorFromId[1].push_back(QColor("#bfc600"));
	colorFromId[1].push_back(QColor("#00c67e"));
	colorFromId[1].push_back(QColor("#e06eb1"));
	colorFromId[1].push_back(QColor("#ff8e57"));

	colorFromId[2].push_back(QColor("#00a5ce"));
	colorFromId[2].push_back(QColor("#ce0500"));
	colorFromId[2].push_back(QColor("#ce00a0"));
	colorFromId[2].push_back(QColor("#1b43ff"));
	colorFromId[2].push_back(QColor("#c2ce00"));
	colorFromId[2].push_back(QColor("#00ce27"));

	colorFromId[3].push_back(QColor("#71b2c5"));
	colorFromId[3].push_back(QColor("#c57177"));
	colorFromId[3].push_back(QColor("#c471c5"));
	colorFromId[3].push_back(QColor("#9571c5"));
	colorFromId[3].push_back(QColor("#c5b671"));
	colorFromId[3].push_back(QColor("#8dc571"));
	colorFromId[3].push_back(QColor("#6d6d6d"));

	colorFromId[4].push_back(QColor("#00ce27"));
	colorFromId[4].push_back(QColor("#00a5ce"));
	colorFromId[4].push_back(QColor("#c2ce00"));
	colorFromId[4].push_back(QColor("#ce00a0"));
	colorFromId[4].push_back(QColor("#1b43ff"));
	colorFromId[4].push_back(QColor("#ce0500"));
}

bool VVVVVVHandler::startElement(const QString & /*namespaceURI*/, const QString & localName, const QString & /*qName*/, const QXmlAttributes & atts)
{
	currentTag = localName;
	currentAttributes = atts;
	
	// those elements don't have contents, so characters() is never called.
	if(currentTag == "edLevelClass")
	{
		QMap<QString, int> tilesetMap;
		tilesetMap["tileset"] = currentAttributes.value("tileset").toInt();
		tilesetMap["id"] = tilesetMap["tileset"] > 1 ? 1 : 0;
		tilesetMap["tilecol"] = currentAttributes.value("tilecol").toInt();
		screenTilesets.push_back(tilesetMap);
	}
	
	return true;
}

bool VVVVVVHandler::characters(const QString & ch)
{
	if(currentTag == "mapwidth")
	{
		mapwidthScreen = ch.toInt();
		mapwidth = mapwidthScreen * SCREEN_WIDTH;
	}
	else if(currentTag == "mapheight")
	{
		mapheightScreen = ch.toInt();
		mapheight = mapheightScreen * SCREEN_HEIGHT;
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
			if(p1 >= 0 && p1 < 4)
			{
				point = QPoint(x, y);
				plainColoredObjects.push_back(point);
			}
			break;
		case 3: // disappear
			{
				point = QPoint(x, y);
				plainColoredObjects.push_back(point);
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
				
				point = QPoint(x*8+3, (y/SCREEN_HEIGHT)*240 + p2*8);
				pixmaps.push_back(qMakePair(point, line));
			}
			else
			{
				QPixmap line(p3, 2);
				line.fill();
				
				point = QPoint((x/SCREEN_WIDTH)*320 + p2*8, y*8+3);
				pixmaps.push_back(qMakePair(point, line));
			}
			break;
		case 13: // mini-teleporters
			// p1 = destination X, p2 = destination Y
			// I use "big" as the entry and "small" and the exit.
			{
				point = QPoint(x*8, y*8);
				whiteObjets.push_back(qMakePair(point, teleportbig));
				point = QPoint(p1*8, p2*8);
				whiteObjets.push_back(qMakePair(point, teleportsmall));
			}
			break;
		case 15: // crew members
			// p1 = 0: cyan / pink / yellow / red / green / 5: blue
			{
				QColor color;
				switch(p1)
				{
					case 0:
						color = QColor(127, 185, 190);
						break;
					case 1:
						color = QColor(208, 113, 203);
						break;
					case 2:
						color = QColor(211, 203, 113);
						break;
					case 3:
						color = QColor(252, 63, 63);
						break;
					case 4:
						color = QColor(94, 213, 113);
						break;
					case 5:
					default:
						color = QColor(75, 75, 230);
				}
				
				point = QPoint(x*8, y*8);
				crew.push_back(qMakePair(point, color));
			}
			break;
		case 16: // captain/start
			{
				// p1 = is looking left ? 1 = <<, 0 = >>
				if(p1)
					rect = QRect(103, 2, 10, 21);
				else
					rect = QRect(7, 2, 10, 21);
				
				point = QPoint(x*8, y*8);
				QPixmap pix = spriteset.copy(QRect(4, 0, 16, 23));
				QBitmap characterMask = pix.mask();
				QPixmap characterChief(QSize(16, 24));
				characterChief.fill(Qt::transparent);
				
				QPainter cPainter;
				cPainter.begin(&characterChief);
				cPainter.setPen(QColor(127, 185, 190));
				cPainter.drawPixmap(0, 1, characterMask);
				cPainter.end();
				
				sprites.push_back(qMakePair(point, characterChief));
			}
			break;
		case 17: // text string
			break;
		case 18: // terminal
			// p1 = is upside down ? 1 = no, 0 = yes
			{
				QBitmap bit(terminalup);
				if(p1) // unsure
					bit = terminaldown;
				
				point = QPoint(x*8, y*8);
				whiteObjets.push_back(qMakePair(point, bit));
			}
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
	QVector<QPixmap> tilesets(2);
	tilesets[0].load("tiles.png");
	tilesets[1].load("tiles2.png");
	
	QPixmap image(mapwidth*8, mapheight*8);
	image.fill(Qt::black);
	
	QPainter p;
	p.begin(&image);
	
	for(int y = 0;y < mapheight;y++)
	{
		for(int x = 0;x < mapwidth;x++)
		{
			int ch = chunks[x+y*mapwidth];
			int xt = ch % SCREEN_WIDTH;
			int yt = ch / SCREEN_WIDTH;
			int screenx = x / SCREEN_WIDTH;
			int screeny = y / SCREEN_HEIGHT;
			int tilesetID = screenTilesets[screenx+screeny*MAX_SCREEN_WIDTH]["id"];
			p.drawPixmap(x*8, y*8, tilesets[tilesetID].copy(xt*8, yt*8, 8, 8));
		}
	}
	
	QListIterator< QPair< QPoint, QPixmap > > i(sprites);
	while(i.hasNext())
	{
		QPair< QPoint, QPixmap > pair = i.next();
		p.drawPixmap(pair.first, pair.second);
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
	
	QListIterator< QPoint > ico(plainColoredObjects);
	while(ico.hasNext())
	{
		QPoint pt = ico.next();

		int screenx = pt.x() / SCREEN_WIDTH;
		int screeny = pt.y() / SCREEN_HEIGHT;
		int id = screenTilesets[screenx+screeny*MAX_SCREEN_WIDTH]["id"];
		int tileset = screenTilesets[screenx+screeny*MAX_SCREEN_WIDTH]["tileset"];
		int tilecol = screenTilesets[screenx+screeny*MAX_SCREEN_WIDTH]["tilecol"];

		QPixmap copy;
		if (id == 0)
		{
			QPoint pos = colorPositionFromId[tilecol];
			QRect srcRect(pos, QSize(8, 8));
			copy = tilesets[0].copy(srcRect);
		}
		else
		{
			if (tileset > 4 || tilecol > colorFromId[tileset].size())
			{
				qDebug() << "ERROR COLOUR" << tileset << ", " << tilecol << ">" << colorFromId[tileset].size();
				copy = tilesets[0];
			}
			else
			{
				copy = QPixmap(8, 8);
				copy.fill(colorFromId[tileset][tilecol]);
			}
		}

		for(int i = 0;i < 4;i++)
		{
			p.drawPixmap((pt.x() + i) * 8, pt.y() * 8, copy);
		}
	}
	
	p.end();
	
	image.save(filename);
	
	qDebug() << "finished";
}

bool VVVVVVHandler::endElement(const QString & /*namespaceURI*/, const QString & /*localName*/, const QString & /*qName*/)
{
	currentTag = QString();
	return true;
}

bool VVVVVVHandler::error(const QXmlParseException & /*exception*/)
{
	return false;
}

bool VVVVVVHandler::fatalError(const QXmlParseException & /*exception*/)
{
	return false;
}

bool VVVVVVHandler::warning (const QXmlParseException & /*exception*/)
{
	return false;
}

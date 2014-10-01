#include "VVVVVVHandler.h"
#include <QApplication>
#include <QFileInfo>

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		qWarning("Error: not enough or too many parameters.");
		return 1;
	}
	
	if(!QFile::exists(argv[1]))
	{
		qWarning("Error: the provided filed does not exists.");
		return 1;
	}
	
	QApplication app(argc, argv);
	
	VVVVVVHandler * handler = new VVVVVVHandler;
	QXmlSimpleReader reader;
	reader.setContentHandler(handler);
	reader.setErrorHandler(handler);
	
	QFile file(argv[1]);
	QXmlInputSource inputSource(&file);
	if(reader.parse(&inputSource))
	{
		QFileInfo fileInfo(file);
		QString fileName(fileInfo.completeBaseName() + ".png");
		handler->saveTo(fileName);
	}
	else
	{
		qWarning("Error: could not parse file.");
	}
	
	return 0;
}

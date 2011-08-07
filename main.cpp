#include "VVVVVVHandler.h"
#include <QApplication>

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
	if(reader.parse(new QXmlInputSource(new QFile(argv[1]))))
	{
		handler->saveTo("output.png");
	}
	else
	{
		qWarning("Error: could not parse file.");
	}
	
	return app.exec();
}

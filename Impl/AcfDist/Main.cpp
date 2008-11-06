// STL includes
#include <iostream>

// Qt includes
#include <QApplication>

#include "CCopyProcessor.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QApplication::setStyle("cleanlooks");

	if (argc == 2){
		CCopyProcessor processor;

		if (processor.ProcessConfigFile(argv[1])){
			std::cout << "All files copied correctly";
		}
		else{
			std::cout << "There are some errors";

			return 1;
		}
	}
	else{
		std::cout << "Usage: AcfDist.exe configurationFile";
	}

	return 0;
}

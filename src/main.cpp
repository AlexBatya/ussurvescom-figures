#include <QApplication>
#include "include/charts.h"

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	Charts w;
	w.show();
	return a.exec();
}


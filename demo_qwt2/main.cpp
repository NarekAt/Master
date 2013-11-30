#include <qt5/QtWidgets/QApplication>
#include <iostream>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    std::cout << "Incorrect argument count.\n";
    std::cout << "Program Usage: grapher file_names" << std::endl;
    return 1;
  }
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}

#include "main_window.h"

#include <QApplication>
#include <QFile>
#include <QtDebug>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  QFile qss(":/style/stylesheet.qss");
  if (qss.open(QFile::ReadOnly)) {
    qDebug() << "Open stylesheet.qss success!";
    QString style = qss.readAll();
    a.setStyleSheet(style);
    qss.close();
  } else {
    qDebug() << "Open stylesheet.qss failed!";
  }

  MainWindow w;
  w.show();

  return a.exec();
}
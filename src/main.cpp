#include "global.h"
#include "main_window.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QSettings>
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

  QString filename = "config.ini";
  QString appPath = QCoreApplication::applicationDirPath();
  QString configPath =
    QDir::toNativeSeparators(appPath + QDir::separator() + filename);
  QSettings settings(configPath, QSettings::IniFormat);
  QString gateHost = settings.value("GateServer/host").toString();
  QString gatePort = settings.value("GateServer/port").toString();
  gateUrlPrefix += "http://" + gateHost + ":" + gatePort;

  MainWindow w;
  w.show();

  return a.exec();
}
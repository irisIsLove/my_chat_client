#include "loading_dialog.h"
#include "ui_loading_dialog.h"

#include <QMovie>

LoadingDialog::LoadingDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::LoadingDialog)
{
  ui->setupUi(this);

  setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint |
                 Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_TranslucentBackground);
  setFixedSize(parent->size());

  QMovie* movie = new QMovie(":/res/loading.gif", {}, this);
  ui->lbLoading->setMovie(movie);
  movie->start();
}

LoadingDialog::~LoadingDialog()
{
  delete ui;
}

#include "chat_dialog.h"
#include "chat_history_widget.h"
#include "ui_chat_dialog.h"

#include <QAction>
#include <QRandomGenerator>

ChatDialog::ChatDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::ChatDialog)
{
  ui->setupUi(this);
  ui->btnSearch->setState("normal", "hover", "press");
  ui->editSearch->setMaxLength(15);

  QAction* actSearch = new QAction(ui->editSearch);
  actSearch->setIcon(QIcon(":/res/search.png"));
  ui->editSearch->addAction(actSearch, QLineEdit::LeadingPosition);
  ui->editSearch->setPlaceholderText("搜索");

  QAction* actClear = new QAction(ui->editSearch);
  actClear->setIcon(QIcon(":/res/close_transparent.png"));
  ui->editSearch->addAction(actClear, QLineEdit::TrailingPosition);
  connect(ui->editSearch,
          &QLineEdit::textChanged,
          [this, actClear](const QString& text) {
            text.isEmpty()
              ? actClear->setIcon(QIcon(":/res/close_transparent.png"))
              : actClear->setIcon(QIcon(":/res/close_search.png"));
          });
  connect(actClear, &QAction::triggered, [this]() {
    ui->editSearch->clear();
    ui->editSearch->clearFocus();
    showUserSearch();
  });

  showUserSearch();
  addTestList();
}

ChatDialog::~ChatDialog()
{
  delete ui;
}
QVector<QString> strs = {
  "hello world !",
  "nice to meet u",
  "New year，new life",
  "You have to love yourself",
  "My love is written in the wind ever since the whole world is you"
};
QVector<QString> heads = { ":/res/head_1.jpg",
                           ":/res/head_2.jpg",
                           ":/res/head_3.jpg",
                           ":/res/head_4.jpg",
                           ":/res/head_5.jpg" };
QVector<QString> names = { "llfc", "zack",   "golang", "cpp",
                           "java", "nodejs", "python", "rust" };
void
ChatDialog::addTestList()
{
  for (int i = 0; i < 13; ++i) {
    int random = QRandomGenerator::global()->bounded(100);
    int str_i = random % strs.size();
    int head_i = random % heads.size();
    int name_i = random % names.size();

    auto* chatHistoryItem = new ChatHistoryWidget(ui->listHistory);
    chatHistoryItem->setInfo(names[name_i], heads[head_i], strs[str_i]);
    QListWidgetItem* item = new QListWidgetItem(ui->listHistory);
    item->setSizeHint(chatHistoryItem->sizeHint());
    ui->listHistory->addItem(item);
    ui->listHistory->setItemWidget(item, chatHistoryItem);
  }
}

void
ChatDialog::showUserSearch(bool isSearch)
{
  if (isSearch) {
    ui->listHistory->hide();
    ui->listContact->hide();
    ui->listSearch->show();
    m_mode = ChatUiMode::MODE_SEARCH;
  } else if (m_state == ChatUiMode::MODE_CHAT) {
    ui->listHistory->show();
    ui->listContact->hide();
    ui->listSearch->hide();
    m_mode = ChatUiMode::MODE_CHAT;
  } else if (m_state == ChatUiMode::MODE_CONTACT) {
    ui->listHistory->hide();
    ui->listContact->show();
    ui->listSearch->hide();
    m_mode = ChatUiMode::MODE_CONTACT;
  }
}

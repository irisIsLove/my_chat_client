#ifndef CHATHISTORYLIST_H
#define CHATHISTORYLIST_H

#include <QListWidget>

class ChatHistoryList : public QListWidget
{
  Q_OBJECT
public:
  explicit ChatHistoryList(QWidget* parent = nullptr);
  ~ChatHistoryList() = default;

signals:
  void sigLoadingHistory();

protected:
  bool eventFilter(QObject* object, QEvent* event) override;

private:
};

#endif // CHATHISTORYLIST_H

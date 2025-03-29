#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QWidget>

class QVBoxLayout;
class QScrollArea;

class ChatView : public QWidget
{
public:
  explicit ChatView(QWidget* parent = nullptr);
  ~ChatView();

  void appendChatItem(QWidget* item);
  void prependChatItem(QWidget* item);
  void insertChatItem(QWidget* before, QWidget* item);

protected:
  bool eventFilter(QObject* watched, QEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

private slots:
  void onVScrollBarMoved(int min, int max);

private:
  void initStyleSheet();

private:
  QVBoxLayout* m_pLayout;
  QScrollArea* m_pScrollArea;
  bool isAppended;
};

#endif // CHATVIEW_H

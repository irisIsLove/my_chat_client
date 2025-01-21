#ifndef CUSTOMIZEEDIT_H
#define CUSTOMIZEEDIT_H

#include <QLineEdit>

class CustomizeEdit : public QLineEdit
{
  Q_OBJECT
public:
  explicit CustomizeEdit(QWidget* parent = nullptr);
  ~CustomizeEdit() = default;
  void setMaxLength(int length);

signals:
  void sigFocusOut();

protected:
  void focusOutEvent(QFocusEvent* e) override;

private slots:
  void limitTextLength(const QString& text);

private:
  int m_maxLength = 0;
};

#endif // CUSTOMIZEEDIT_H

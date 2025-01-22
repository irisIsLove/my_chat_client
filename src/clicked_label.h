#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>

enum class ClickedLbState
{
  Normal,
  Selected,
};

class ClickedLabel : public QLabel
{
  Q_OBJECT
public:
  explicit ClickedLabel(QWidget* parent = nullptr);
  ~ClickedLabel() = default;

  void setState(const QString& normal = "",
                const QString& hover = "",
                const QString& clicked = "",
                const QString& selected = "",
                const QString& selectedHover = "",
                const QString& selectedClicked = "");
  ClickedLbState getState() const;

protected:
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;
  void mousePressEvent(QMouseEvent* ev) override;
  void mouseReleaseEvent(QMouseEvent* ev) override;

signals:
  void sigClicked();

private:
  QString m_qsNorml, m_qsHover, m_qsClicked;
  QString m_qsSelected, m_qsSelectedHover, m_qsSelectedClicked;
  ClickedLbState m_curState = ClickedLbState::Normal;
};

#endif // CLICKEDLABEL_H

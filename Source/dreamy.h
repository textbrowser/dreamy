#ifndef _dreamy_h_
#define _dreamy_h_

#include <QApplication>
#include <QShortcut>
#include <QWidget>
#include <QtDebug>

class dreamy: public QWidget
{
  Q_OBJECT

 public:
  dreamy(void)
  {
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q),
		  this,
		  SLOT(slot_quit(void)));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W),
		  this,
		  SLOT(slot_quit(void)));
    setWindowFlags(Qt::Dialog |
		   Qt::FramelessWindowHint |
		   Qt::Popup |
		   Qt::WindowStaysOnTopHint);
  }

  ~dreamy()
  {
  }

 private slots:
  void slot_quit(void)
  {
    QApplication::exit(0);
  }
};

#endif

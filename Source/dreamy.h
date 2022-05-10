#ifndef _dreamy_h_
#define _dreamy_h_

#include <QApplication>
#include <QShortcut>
#include <QWidget>
#include <QtDebug>

#include "dreamy_options.h"
#include "ui_dreamy.h"

class dreamy: public QWidget
{
  Q_OBJECT

 public:
  dreamy(void)
  {
    m_options = nullptr;
    m_ui.setupUi(this);
    connect(m_ui.options,
	    &QPushButton::clicked,
	    this,
	    &dreamy::slot_options);
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

 private:
  Ui_dreamy m_ui;
  dreamy_options *m_options;

 private slots:
  void slot_options(void)
  {
    if(!m_options)
      {
	m_options = new dreamy_options(this);
	m_options->setModal(false);
      }

    m_options->show();
  }

  void slot_quit(void)
  {
    QApplication::exit(0);
  }
};

#endif

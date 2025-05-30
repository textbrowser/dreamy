/*
** Copyright (c) 2022, Alexis Megas.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from Dreamy without specific prior written permission.
**
** DREAMY IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** DREAMY, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _dreamy_h_
#define _dreamy_h_

#include <QApplication>
#include <QDateTime>
#ifdef Q_OS_ANDROID
#if (QT_VERSION >= QT_VERSION_CHECK(6, 1, 0))
#include <QJniObject>
#endif
#endif
#include <QShortcut>
#include <QTimer>

#include "dreamy_options.h"
#include "ui_dreamy.h"

class dreamy: public QMainWindow
{
  Q_OBJECT

 public:
  dreamy(void):QMainWindow()
  {
    m_cursor_timer.setSingleShot(true);
    m_cursor_timer.start(5000);
#ifdef Q_OS_ANDROID
    m_options = new dreamy_options(nullptr);
#else
    m_options = new dreamy_options(this);
#endif
    m_options->setModal(false);
    m_timer.start(1000);
    m_ui.setupUi(this);
    m_ui.date->setVisible(true);
    connect(&m_cursor_timer,
	    &QTimer::timeout,
	    this,
	    &dreamy::slot_hide_cursor);
    connect(&m_timer,
	    &QTimer::timeout,
	    this,
	    &dreamy::slot_tick);
    connect(m_options,
	    &dreamy_options::accepted,
	    this,
	    &dreamy::slot_options_accepted);
    connect(m_ui.options,
	    &QPushButton::clicked,
	    this,
	    &dreamy::slot_options);
    connect(m_ui.quit,
	    &QPushButton::clicked,
	    this,
	    &dreamy::slot_quit);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G),
		  this,
		  SLOT(slot_options(void)));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q),
		  this,
		  SLOT(slot_quit(void)));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W),
		  this,
		  SLOT(slot_quit(void)));
#else
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_G),
		  this,
		  SLOT(slot_options(void)));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q),
		  this,
		  SLOT(slot_quit(void)));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_W),
		  this,
		  SLOT(slot_quit(void)));
#endif
    setStyleSheet(QString("QWidget#dreamy {background-color: %1;}").
		  arg(m_options->background_color().name()));
    setToolTip(tr("Press Ctrl+G for Settings. "
		  "Press Ctrl+Q or Ctrl+W to exit Dreamy."));
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    slot_options_accepted();
  }

  ~dreamy()
  {
#ifdef Q_OS_ANDROID
    m_options->deleteLater();
#endif
  }

 private:
  QString m_colon;
  QTimer m_cursor_timer;
  QTimer m_timer;
  Ui_dreamy m_ui;
  dreamy_options *m_options;

  bool eventFilter(QObject *object, QEvent *event)
  {
    Q_UNUSED(object);

    if(event && event->type() == QEvent::MouseMove)
      {
	m_cursor_timer.start();
	setCursor(QCursor(Qt::ArrowCursor));
      }

    return false;
  }

  void mouseDoubleClickEvent(QMouseEvent *event)
  {
    QMainWindow::mouseDoubleClickEvent(event);
    slot_options();
  }

 private slots:
  void slot_hide_cursor(void)
  {
    setCursor(QCursor(Qt::BlankCursor));
  }

  void slot_options(void)
  {
#ifdef Q_OS_ANDROID
    m_options->showMaximized();
#else
    m_options->resize(m_options->sizeHint());
    m_options->show();
#endif
  }

  void slot_options_accepted(void)
  {
    m_ui.clock_layout->removeWidget(m_ui.date);
    m_ui.clock_layout->removeWidget(m_ui.time);

    auto font(m_options->font());

    font.setPointSize(font.pointSize() / 3);
    m_ui.date->setFont(font);
    m_ui.date->setStyleSheet
      (QString("QLabel {color: %1;}").arg(m_options->font_color().name()));
    m_ui.date->setVisible(m_options->show_date());
    m_ui.date->set_angle(m_options->angle());
    m_ui.options->setStyleSheet(QString("QWidget {background-color: %1;}").
				arg(m_options->background_color().name()));
    m_ui.quit->setStyleSheet(QString("QWidget {background-color: %1;}").
			     arg(m_options->background_color().name()));
    m_ui.time->setFont(m_options->font());
    m_ui.time->setStyleSheet
      (QString("QLabel {color: %1;}").arg(m_options->font_color().name()));
    m_ui.time->set_angle(m_options->angle());
    setStyleSheet(QString("QWidget#dreamy {background-color: %1;}").
		  arg(m_options->background_color().name()));

    if(m_options->angle() == 0)
      {
	m_ui.clock_layout->addWidget(m_ui.time);
	m_ui.clock_layout->addWidget(m_ui.date);
      }
    else
      {
	m_ui.clock_layout->addWidget(m_ui.date);
	m_ui.clock_layout->addWidget(m_ui.time);
      }
  }

  void slot_quit(void)
  {
    QApplication::exit(0);
#ifdef Q_OS_ANDROID
#if (QT_VERSION >= QT_VERSION_CHECK(6, 1, 0))
    auto activity = QJniObject
      (QNativeInterface::QAndroidApplication::context());

    activity.callMethod<void> ("finishAndRemoveTask");
#endif
#endif
  }

  void slot_tick(void)
  {
    QSettings settings
      (dreamy_options::settings_filename(), QSettings::IniFormat);
    auto const now(QDateTime::currentDateTime());
    auto const s = now.toString("ss").toInt();

    if(settings.value("blank_screen", true).toBool() && s >= 0 && s <= 2)
      {
	m_timer.setInterval(5000);
	m_ui.date->setVisible(false);
#ifndef Q_OS_ANDROID
	m_ui.options->setVisible(false);
	m_ui.quit->setVisible(false);
#endif
	m_ui.time->setVisible(false);
	return;
      }
    else
      {
	if(!m_ui.date->isVisible())
	  m_ui.date->setVisible(m_options->show_date());

	if(!m_ui.time->isVisible())
	  m_ui.time->setVisible(true);

	if(m_timer.interval() != 1000)
	  m_timer.setInterval(1000);

	m_ui.options->setVisible(true);
	m_ui.quit->setVisible(true);
      }

    if(m_colon == " ")
      m_colon = ":";
    else
      m_colon = " ";

    QString const seconds(m_options->show_seconds() ? m_colon + "ss" : "");

    if(m_options->show_am_pm())
      m_ui.time->setText(now.toString("h" + m_colon + "mm" + seconds + " AP"));
    else
      m_ui.time->setText
	((now.toString("h" + m_colon + "mm" + seconds + " AP").
	  mid(0, 5 + seconds.length())));

    if(m_options->show_date() &&
       m_ui.date->text() != now.toString("yyyy-MM-dd"))
      m_ui.date->setText(now.toString("yyyy-MM-dd"));
  }
};

#endif

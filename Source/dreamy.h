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
#include <QShortcut>
#include <QTimer>
#include <QWidget>

#include "dreamy_options.h"
#include "ui_dreamy.h"

class dreamy: public QWidget
{
  Q_OBJECT

 public:
  dreamy(void):QWidget()
  {
    m_options = new dreamy_options(this);
    m_options->setModal(false);
    m_timer.start(1000);
    m_ui.setupUi(this);
    m_ui.date->setVisible(false);
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
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G),
		  this,
		  SLOT(slot_options(void)));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q),
		  this,
		  SLOT(slot_quit(void)));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W),
		  this,
		  SLOT(slot_quit(void)));
    setStyleSheet(QString("QWidget#dreamy {background-color: %1;}").
		  arg(m_options->background_color().name()));
    setWindowFlags(Qt::Dialog |
		   Qt::FramelessWindowHint |
		   Qt::Popup |
		   Qt::WindowStaysOnTopHint);
    slot_options_accepted();
  }

  ~dreamy()
  {
  }

 private:
  QTimer m_timer;
  Ui_dreamy m_ui;
  dreamy_options *m_options;

 private slots:
  void slot_options(void)
  {
    m_options->show();
  }

  void slot_options_accepted(void)
  {
    m_ui.clock_layout->removeWidget(m_ui.date);
    m_ui.clock_layout->removeWidget(m_ui.time);

    auto font(m_options->font());

    font.setPointSize(font.pointSize() / 4);
    m_ui.date->setFont(font);
    m_ui.date->setStyleSheet
      (QString("QLabel {color: %1;}").arg(m_options->font_color().name()));
    m_ui.date->setVisible(m_options->show_date());
    m_ui.date->set_angle(m_options->angle());
    m_ui.options->setStyleSheet(QString("QWidget {background-color: %1;}").
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
  }

  void slot_tick(void)
  {
    QString seconds(m_options->show_seconds() ? ":ss" : "");
    auto now(QDateTime::currentDateTime());

    if(m_options->show_am_pm())
      m_ui.time->setText(now.toString("h:mm" + seconds + " AP"));
    else
      m_ui.time->setText
	((now.toString("h:mm" + seconds + " AP").
	  mid(0, 5 + seconds.length())));

    if(m_options->show_date())
      m_ui.date->setText(now.toString("yyyy-MM-dd"));
  }
};

#endif

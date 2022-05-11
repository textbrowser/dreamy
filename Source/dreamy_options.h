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

#ifndef _dreamy_options_h_
#define _dreamy_options_h_

#include <QColorDialog>
#include <QDir>
#include <QFontDialog>
#include <QSettings>

#include "ui_dreamy_options.h"

class dreamy_options: public QDialog
{
  Q_OBJECT

 public:
  dreamy_options(QWidget *parent):QDialog(parent)
  {
    m_ui.setupUi(this);
    connect(m_ui.background_color,
	    &QPushButton::clicked,
	    this,
	    &dreamy_options::slot_color_button_clicked);
    connect(m_ui.font,
	    &QPushButton::clicked,
	    this,
	    &dreamy_options::slot_font_button_clicked);
    connect(m_ui.font_color,
	    &QPushButton::clicked,
	    this,
	    &dreamy_options::slot_color_button_clicked);
    connect(m_ui.font_size,
	    QOverload<int>::of(&QSpinBox::valueChanged),
	    this,
	    &dreamy_options::slot_point_size_changed);
    connect(m_ui.show_am_pm,
	    QOverload<int>::of(&QCheckBox::stateChanged),
	    this,
	    &dreamy_options::slot_checkbox_clicked);
    connect(m_ui.show_date,
	    QOverload<int>::of(&QCheckBox::stateChanged),
	    this,
	    &dreamy_options::slot_checkbox_clicked);
    connect(m_ui.show_seconds,
	    QOverload<int>::of(&QCheckBox::stateChanged),
	    this,
	    &dreamy_options::slot_checkbox_clicked);
    restore_settings();
  }

  QColor background_color(void) const
  {
    return QColor(m_ui.background_color->text());
  }

  QColor font_color(void) const
  {
    return QColor(m_ui.font_color->text());
  }

  QFont font(void) const
  {
    QFont font;

    font.fromString(m_ui.font->text());
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPointSize(m_ui.font_size->value());
    font.setStyleStrategy(QFont::PreferAntialias);
    return font;
  }

  bool show_am_pm(void) const
  {
    return m_ui.show_am_pm->isChecked();
  }

  bool show_date(void) const
  {
    return m_ui.show_date->isChecked();
  }

  bool show_seconds(void) const
  {
    return m_ui.show_seconds->isChecked();
  }

 private:
  Ui_dreamy_options m_ui;

  QString settings_filename(void) const
  {
    QDir dir;

    dir.mkdir(QDir::homePath() + QDir::separator() + ".dreamy");
    return QDir::homePath() +
      QDir::separator() +
      ".dreamy" +
      QDir::separator() +
      "dreamy.ini";
  }

  void restore_settings(void)
  {
    QSettings settings(settings_filename(), QSettings::IniFormat);

    m_ui.background_color->setStyleSheet
      (QString("QPushButton {background-color: %1;}").
       arg(settings.value("background_color").toString().mid(0, 25)));
    m_ui.background_color->setText
      (settings.value("background_color").toString().mid(0, 25));

    QFont font;

    if(!font.fromString(settings.value("font").toString().mid(0, 100)))
      font = QApplication::font();

    m_ui.font->setText(font.toString());
    m_ui.font_color->setStyleSheet
      (QString("QPushButton {background-color: %1;}").
       arg(settings.value("font_color").toString().mid(0, 25)));
    m_ui.font_color->setText
      (settings.value("font_color").toString().mid(0, 25));
    m_ui.font_size->blockSignals(true);
    m_ui.font_size->setValue(settings.value("font_size").toInt());
    m_ui.font_size->blockSignals(false);
    m_ui.show_am_pm->blockSignals(true);
    m_ui.show_am_pm->setChecked(settings.value("show_am_pm").toBool());
    m_ui.show_am_pm->blockSignals(false);
    m_ui.show_date->blockSignals(true);
    m_ui.show_date->setChecked(settings.value("show_date").toBool());
    m_ui.show_date->blockSignals(false);
    m_ui.show_seconds->blockSignals(true);
    m_ui.show_seconds->setChecked(settings.value("show_seconds").toBool());
    m_ui.show_seconds->blockSignals(false);
  }

  void save_settings(void)
  {
    QSettings settings(settings_filename(), QSettings::IniFormat);

    settings.setValue("background_color", m_ui.background_color->text());
    settings.setValue("font", m_ui.font->text());
    settings.setValue("font_color", m_ui.font_color->text());
    settings.setValue("font_size", m_ui.font_size->value());
    settings.setValue("show_am_pm", m_ui.show_am_pm->isChecked());
    settings.setValue("show_date", m_ui.show_date->isChecked());
    settings.setValue("show_seconds", m_ui.show_seconds->isChecked());
  }

 private slots:
  void slot_checkbox_clicked(int state)
  {
    Q_UNUSED(state);
    save_settings();
    emit accepted();
  }

  void slot_color_button_clicked(void)
  {
    auto button = qobject_cast<QPushButton *> (sender());

    if(!button)
      return;

    QColorDialog dialog(this);

    dialog.setCurrentColor(QColor(button->text()));
    dialog.setWindowIcon(windowIcon());
    dialog.setWindowTitle(tr("Dreamy: Select Color"));

    if(dialog.exec() == QDialog::Accepted)
      {
	button->setStyleSheet
	  (QString("QPushButton {background-color: %1;}").
	   arg(dialog.selectedColor().name()));
	button->setText(dialog.selectedColor().name());
	save_settings();
	emit accepted();
      }
  }

  void slot_font_button_clicked(void)
  {
    auto button = qobject_cast<QPushButton *> (sender());

    if(!button)
      return;

    QFont font;
    QFontDialog dialog(this);

    font.fromString(button->text());
    dialog.setCurrentFont(font);
    dialog.setWindowIcon(windowIcon());
    dialog.setWindowTitle(tr("Dreamy: Select Font"));

    if(dialog.exec() == QDialog::Accepted)
      {
	button->setText(dialog.selectedFont().toString());
	save_settings();
	emit accepted();
      }
  }

  void slot_point_size_changed(int value)
  {
    auto font(this->font());

    font.setPointSize(value);
    m_ui.font->setText(font.toString());
    save_settings();
    emit accepted();
  }

 signals:
   void accepted(void);
};

#endif

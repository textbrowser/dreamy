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

#include "dreamy_version.h"
#include "ui_dreamy_options.h"

class dreamy_options: public QDialog
{
  Q_OBJECT

 public:
  static QString settings_filename(void)
  {
    QDir dir;

    dir.mkdir(QDir::homePath() + QDir::separator() + ".dreamy");
    return QDir::homePath() +
      QDir::separator() +
      ".dreamy" +
      QDir::separator() +
      "dreamy.ini";
  }

  dreamy_options(QWidget *parent):QDialog(parent)
  {
    m_ui.setupUi(this);
    m_ui.version->setText(tr("Dreamy Version %1").arg(DREAMY_VERSION));
    connect(m_ui.background_color,
	    &QPushButton::clicked,
	    this,
	    &dreamy_options::slot_color_button_clicked);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 7, 0))
    connect(m_ui.blank_screen,
	    QOverload<Qt::CheckState>::of(&QCheckBox::checkStateChanged),
	    this,
	    &dreamy_options::slot_checkbox_clicked);
#else
    connect(m_ui.blank_screen,
	    QOverload<int>::of(&QCheckBox::stateChanged),
	    this,
	    &dreamy_options::slot_checkbox_clicked);
#endif    
#ifdef Q_OS_ANDROID
    connect(m_ui.button_box->button(QDialogButtonBox::Close),
	    &QPushButton::clicked,
	    this,
	    &dreamy_options::hide);
#else
    connect(m_ui.button_box->button(QDialogButtonBox::Close),
	    &QPushButton::clicked,
	    this,
	    &dreamy_options::close);
#endif
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
    connect(m_ui.rotate,
	    &QPushButton::clicked,
	    this,
	    &dreamy_options::slot_rotate);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 7, 0))
    connect(m_ui.show_am_pm,
	    QOverload<Qt::CheckState>::of(&QCheckBox::checkStateChanged),
	    this,
	    &dreamy_options::slot_checkbox_clicked);
    connect(m_ui.show_date,
	    QOverload<Qt::CheckState>::of(&QCheckBox::checkStateChanged),
	    this,
	    &dreamy_options::slot_checkbox_clicked);
    connect(m_ui.show_seconds,
	    QOverload<Qt::CheckState>::of(&QCheckBox::checkStateChanged),
	    this,
	    &dreamy_options::slot_checkbox_clicked);
#else
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
#endif
    restore_settings();
  }

  QColor background_color(void) const
  {
    return QColor(m_ui.background_color->text().remove('&'));
  }

  QColor font_color(void) const
  {
    return QColor(m_ui.font_color->text().remove('&'));
  }

  QFont font(void) const
  {
    QFont font;

    if(m_ui.font->text().remove('&').trimmed().isEmpty() ||
       !font.fromString(m_ui.font->text().remove('&')))
      font = QApplication::font();

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

  int angle(void) const
  {
    return m_ui.angle->value();
  }

 private:
  Ui_dreamy_options m_ui;

  void restore_settings(void)
  {
    QSettings settings(settings_filename(), QSettings::IniFormat);

    m_ui.angle->setValue(settings.value("angle").toInt());
    m_ui.background_color->setStyleSheet
      (QString("QPushButton {background-color: %1;}").
       arg(settings.value("background_color", m_ui.background_color->text()).
	   toString().trimmed().mid(0, 25)));
    m_ui.background_color->setText
      (settings.value("background_color", m_ui.background_color->text()).
       toString().remove('&').trimmed().mid(0, 25));
    m_ui.blank_screen->blockSignals(true);
    m_ui.blank_screen->setChecked
      (settings.value("blank_screen", true).toBool());
    m_ui.blank_screen->blockSignals(false);

    QFont font;
    auto const string(settings.value("font").toString().mid(0, 250));

    if(string.trimmed().isEmpty() || !font.fromString(string))
      font = QApplication::font();

    font.setPointSize
      (qBound(m_ui.font_size->minimum(),
	      settings.value("font_size").toInt(),
	      m_ui.font_size->maximum()));
    m_ui.font->setText(font.toString());
    m_ui.font_color->setStyleSheet
      (QString("QPushButton {background-color: %1;}").
       arg(settings.value("font_color", m_ui.font_color->text()).
	   toString().trimmed().mid(0, 25)));
    m_ui.font_color->setText
      (settings.value("font_color", m_ui.font_color->text()).
       toString().remove('&').trimmed().mid(0, 25));
    m_ui.font_size->blockSignals(true);
    m_ui.font_size->setValue(settings.value("font_size").toInt());
    m_ui.font_size->blockSignals(false);
    m_ui.show_am_pm->blockSignals(true);
    m_ui.show_am_pm->setChecked(settings.value("show_am_pm", true).toBool());
    m_ui.show_am_pm->blockSignals(false);
    m_ui.show_date->blockSignals(true);
    m_ui.show_date->setChecked(settings.value("show_date", true).toBool());
    m_ui.show_date->blockSignals(false);
    m_ui.show_seconds->blockSignals(true);
    m_ui.show_seconds->setChecked
      (settings.value("show_seconds", true).toBool());
    m_ui.show_seconds->blockSignals(false);
  }

  void save_settings(void)
  {
    QSettings settings(settings_filename(), QSettings::IniFormat);

    settings.setValue("angle", m_ui.angle->value());
    settings.setValue
      ("background_color", m_ui.background_color->text().remove('&'));
    settings.setValue("blank_screen", m_ui.blank_screen->isChecked());
    settings.setValue("font", m_ui.font->text().remove('&'));
    settings.setValue("font_color", m_ui.font_color->text().remove('&'));
    settings.setValue("font_size", m_ui.font_size->value());
    settings.setValue("show_am_pm", m_ui.show_am_pm->isChecked());
    settings.setValue("show_date", m_ui.show_date->isChecked());
    settings.setValue("show_seconds", m_ui.show_seconds->isChecked());
  }

 private slots:
#if (QT_VERSION >= QT_VERSION_CHECK(6, 7, 0))
  void slot_checkbox_clicked(Qt::CheckState state)
#else
  void slot_checkbox_clicked(int state)
#endif
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

    dialog.setCurrentColor(QColor(button->text().remove('&')));
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

    if(button->text().remove('&').trimmed().isEmpty() ||
       !font.fromString(button->text().remove('&')))
      font = QApplication::font();

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

  void slot_rotate(void)
  {
    if(m_ui.angle->value() == 0)
      m_ui.angle->setValue(180);
    else
      m_ui.angle->setValue(0);

    save_settings();
    emit accepted();
  }

 signals:
   void accepted(void);
};

#endif

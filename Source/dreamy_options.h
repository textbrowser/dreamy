#ifndef _dreamy_options_h_
#define _dreamy_options_h_

#include "ui_dreamy_options.h"

class dreamy_options: public QDialog
{
  Q_OBJECT

 public:
  dreamy_options(QWidget *parent):QDialog(parent)
  {
    m_ui.setupUi(this);
  }

 private:
  Ui_dreamy_options m_ui;
};

#endif

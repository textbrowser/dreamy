#include "dreamy.h"

int main(int argc, char *argv[])
{
  QApplication qapplication(argc, argv);
  dreamy d;

  d.show();
  return qapplication.exec();
}

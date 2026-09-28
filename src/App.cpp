
#include "App.hpp"
#include <QWidget>
#include <QApplication>
#include "EmulatorWindow.hpp"

using namespace std;
using namespace chip8;

int runQt6App(AppContext &context) {
  QApplication app(context.argc, const_cast<char**>(context.argv));
  QtEmulatorWindow window(nullptr, context);
  window.show();
  context.window_ready.set_value();
  return app.exec();
}


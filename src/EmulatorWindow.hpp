#include "App.hpp"
#include "Color.hpp"
#include "chip8.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qwidget.h>

class EmulatorRenderer : public QWidget {
  Q_OBJECT

public:
  explicit EmulatorRenderer(QWidget *parent, chip8::Framebuffer framebuffer);

public slots:
  void setFrame(chip8::Framebuffer framebuffer) {
    m_framebuffer = framebuffer;
    update();
  }

  void setColorScheme(ColorScheme cs) { m_colorscheme = cs; update(); }

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  chip8::Framebuffer m_framebuffer;
  ColorScheme m_colorscheme{ColorScheme::Default};
};

class QtEmulatorWindow : public QWidget {
  Q_OBJECT
  enum { DefaultWidth = 600, DefaultHeight = DefaultWidth / 2 };

public:
  explicit QtEmulatorWindow(QWidget *parent, AppContext &context);
  ~QtEmulatorWindow() override;

signals:
  void redraw(chip8::Framebuffer framebuffer);

private:
  QVBoxLayout *m_layout;
  EmulatorRenderer *m_renderer;
  AppContext &m_context;
};

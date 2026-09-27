#include "App.hpp"
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
    counter++;
    m_label->setText(QString("Counter: %1").arg(counter));
    update();
  }

protected:
  void paintEvent(QPaintEvent event);

private:
  chip8::Framebuffer m_framebuffer;
  QLabel *m_label;
  int counter{0};
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

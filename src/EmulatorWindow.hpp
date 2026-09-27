#pragma once
#include "App.hpp"
#include "Color.hpp"
#include "chip8.hpp"
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

class EmulatorSource : public QObject {
    Q_OBJECT
    public:
        explicit EmulatorSource(AppContext &context, QObject *parent = nullptr);
        ~EmulatorSource();

    signals:
        void emulatorStateChanged(chip8::EmulatorState st);
        void emulatorRedraw(chip8::Framebuffer fb);
        void keypadStateChanged(chip8::Keypad keypad);
        void registersChanged(chip8::Registers registers);
        void ispChanged(uint16_t isp);
        void clockSpeedChanged(uint64_t clockSpeed);

    private:
        AppContext &m_context;
};

class EmulatorDebugger : public QWidget {
  Q_OBJECT 

  public:
    explicit EmulatorDebugger(QWidget *parent = nullptr);

  public slots:
    void setRegisters(chip8::Registers reg) { m_reg = reg; render_(); }
    void setIsp(uint16_t isp) { m_isp = isp; render_(); }
  
  private:
    void render_();
    QLabel *m_label;
    QVBoxLayout *m_layout;
    chip8::Registers m_reg{};
    uint16_t m_isp{0};
};

class EmulatorToolbar : public QToolBar {
  Q_OBJECT

public:
  explicit EmulatorToolbar(QWidget *parent = nullptr);

public slots:
  void setEmulatorState(chip8::EmulatorState st) {
    m_state = st;
    renderStateLabel_();
  }

private:
  chip8::EmulatorState m_state{chip8::EmulatorState::Halted};
  void renderStateLabel_();
  QLabel *m_stateLabel;
};

class EmulatorRenderer : public QWidget {
  Q_OBJECT

public:
  explicit EmulatorRenderer(QWidget *parent, chip8::Framebuffer framebuffer);

public slots:
  void setFrame(chip8::Framebuffer framebuffer) {
    m_framebuffer = framebuffer;
    update();
  }

  void setColorScheme(ColorScheme cs) {
    m_colorscheme = cs;
    update();
  }

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
  void stateChanged(chip8::EmulatorState st);

private:
  QVBoxLayout *m_layout;
  EmulatorRenderer *m_renderer;
  EmulatorToolbar *m_toolbar;
  EmulatorDebugger *m_debugger;
  AppContext &m_context;
  EmulatorSource *m_emulator;
};

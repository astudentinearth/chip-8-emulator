#include "App.hpp"
#include "EmulatorWindow.hpp"
#include "VCenterBox.hpp"
#include "chip8.hpp"
#include <QComboBox>
#include <QStyle>
#include <QToolButton>

EmulatorToolbar::EmulatorToolbar(AppContext &context, QtEmulatorWindow *parent,
                                 EmulatorSource *source)
    : QToolBar(parent), m_context{context} {

  setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
  setIconSize(QSize(16, 16));
  m_source = source;
  auto *togglePause = addAction("Pause");
  togglePause->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));

  addSeparator();
  auto *debugger = addAction("Debugger");
  debugger->setCheckable(true);
  debugger->setChecked(context.debuggerVisible);

  connect(debugger, &QAction::toggled, parent,
          &QtEmulatorWindow::setDebuggerVisibility);

  addSeparator();

  auto clockspeedLabel = new QLabel(this);
  clockspeedLabel->setText("Clock speed:");
  clockspeedLabel->setAlignment(Qt::AlignVCenter);
  addWidget(clockspeedLabel);
  auto *clockspeedMenu = new QComboBox(this);
  clockspeedMenu->addItems({"500", "750", "1000", "2000"});
  clockspeedMenu->setCurrentIndex(0);
  clockspeedMenu->setToolTip(
      "Adjust this if moving things with the keypad feels too slow/fast.\nThis "
      "doesn't have an effect on timers (they will keep running at 60Hz).");
  addWidget(new VCenterBox(this, clockspeedMenu));

  connect(clockspeedMenu, &QComboBox::currentTextChanged,
          [parent](const QString &hz) { parent->setClockSpeed(hz.toULong()); });

  connect(
      source, &EmulatorSource::emulatorStateChanged, this,
      [togglePause, source](chip8::EmulatorState st) {
        if (st != chip8::EmulatorState::Halted) {
          togglePause->setIcon(
              QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));
          togglePause->setText("Pause");
          disconnect(togglePause, &QAction::triggered, source,
                     &EmulatorSource::runEmulator);
          connect(togglePause, &QAction::triggered, source,
                  &EmulatorSource::pauseEmulator);
        } else {
          togglePause->setIcon(
              QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
          togglePause->setText("Run");
          disconnect(togglePause, &QAction::triggered, source,
                     &EmulatorSource::pauseEmulator);
          connect(togglePause, &QAction::triggered, source,
                  &EmulatorSource::runEmulator);
        }
      },
      Qt::QueuedConnection);

}

#include "App.hpp"
#include "EmulatorWindow.hpp"
#include <QToolButton>
#include <QStyle>
#include <QComboBox>
#include <QtCore/qnamespace.h>

EmulatorToolbar::EmulatorToolbar(AppContext &context, QtEmulatorWindow *parent) : QToolBar(parent), m_context{context} {
  m_stateLabel = new QLabel(this);
  m_stateLabel->setContentsMargins(8,8,8,8);
  addWidget(m_stateLabel);

  addSeparator();
  auto *debugger = addAction("Debugger");
  debugger->setCheckable(true);
  debugger->setChecked(context.debuggerVisible);

  connect(debugger, &QAction::toggled, parent, &QtEmulatorWindow::setDebuggerVisibility);

  addSeparator();

  auto clockspeedLabel = new QLabel(this);
  clockspeedLabel->setText("Clock speed:");
  clockspeedLabel->setAlignment(Qt::AlignVCenter);
  addWidget(clockspeedLabel);
  auto *clockspeedMenu = new QComboBox(this);
  clockspeedMenu->addItems({"500", "750", "1000", "2000"});
  clockspeedMenu->setCurrentIndex(0);
  addWidget(clockspeedMenu);
  
  connect(clockspeedMenu, &QComboBox::currentTextChanged, [parent](const QString& hz){
    parent->setClockSpeed(hz.toULong());
      });

  renderStateLabel_();
}

void EmulatorToolbar::renderStateLabel_() {
  std::ostringstream stream{};
  stream << m_state;
  m_stateLabel->setText(QString::fromStdString(stream.str()));
}

#include "App.hpp"
#include "EmulatorWindow.hpp"
#include <QToolButton>
#include <QStyle>

EmulatorToolbar::EmulatorToolbar(AppContext &context, QWidget *parent) : QToolBar(parent), m_context{context} {
  m_stateLabel = new QLabel(this);
  m_stateLabel->setContentsMargins(8,8,8,8);
  addWidget(m_stateLabel);

  auto *debugger = addAction("Debugger");
  debugger->setCheckable(true);
  debugger->setChecked(context.debuggerVisible);

  connect(debugger, &QAction::toggled, dynamic_cast<QtEmulatorWindow*>(parent), &QtEmulatorWindow::setDebuggerVisibility);

  renderStateLabel_();
}

void EmulatorToolbar::renderStateLabel_() {
  std::ostringstream stream{};
  stream << m_state;
  m_stateLabel->setText(QString::fromStdString(stream.str()));
}

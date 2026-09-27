#include "EmulatorWindow.hpp"

EmulatorToolbar::EmulatorToolbar(QWidget *parent) : QToolBar(parent) {
  m_stateLabel = new QLabel(this);
  m_stateLabel->setContentsMargins(8,8,8,8);
  addWidget(m_stateLabel);
  renderStateLabel_();
}

void EmulatorToolbar::renderStateLabel_() {
  std::ostringstream stream{};
  stream << m_state;
  m_stateLabel->setText(QString::fromStdString(stream.str()));
}

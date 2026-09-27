
#include "EmulatorWindow.hpp"
#include <QFontDatabase>

EmulatorDebugger::EmulatorDebugger(QWidget *parent): QWidget(parent) {
    m_layout = new QVBoxLayout(this);
    m_label = new QLabel(this);
    const QFont mono = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    m_label->setFont(mono);
    m_label->setContentsMargins(0,0,0,0);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->addWidget(m_label);
}

void EmulatorDebugger::render_() {
    auto text = QString(
            "v0 %1 | v1 %2 | v2 %3 | v3 %4 | v4 %5 | v5 %6 | v6 %7\n"
            "v7 %8 | v8 %9 | v9 %10 | va %11 | vb %12 | vc %13 | vd %14 | ve %15\n"
            "vf %16 | isp %17 | dt %18 | st %19"
            )
        .arg(m_reg.v0)
        .arg(m_reg.v1)
        .arg(m_reg.v2)
        .arg(m_reg.v3)
        .arg(m_reg.v4)
        .arg(m_reg.v5)
        .arg(m_reg.v6)
        .arg(m_reg.v7)
        .arg(m_reg.v8)
        .arg(m_reg.v9)
        .arg(m_reg.va)
        .arg(m_reg.vb)
        .arg(m_reg.vc)
        .arg(m_reg.vd)
        .arg(m_reg.ve)
        .arg(m_reg.vf)
        .arg(m_isp)
        .arg(m_reg.dt)
        .arg(m_reg.st);

    m_label->setText(text);
}




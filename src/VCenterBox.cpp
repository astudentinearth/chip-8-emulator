
#include "VCenterBox.hpp"

VCenterBox::VCenterBox(QWidget *parent, QWidget *item) : QWidget(parent) {
  auto layout = new QHBoxLayout(this);
  layout->addWidget(item, 0, Qt::AlignVCenter);
  layout->setContentsMargins(0, 0, 0, 0);
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}

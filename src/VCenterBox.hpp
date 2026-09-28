
#include <QHBoxLayout>
#include <QWidget>

class VCenterBox : public QWidget {
  Q_OBJECT
public:
  explicit VCenterBox(QWidget *parent, QWidget *item);
};

#pragma once
#include <QWidget>

namespace Ui {
    class TracksPage;
}
class Core;

class TracksPage : public QWidget {
    Q_OBJECT

public:
    TracksPage(Core& core, QWidget* parent=nullptr);
    ~TracksPage();

private:
    Ui::TracksPage* ui;
    Core& core;
};
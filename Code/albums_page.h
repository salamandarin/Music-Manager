#pragma once
#include <QWidget>

namespace Ui {
    class AlbumsPage;
}
class Core;

class AlbumsPage : public QWidget {
    Q_OBJECT

public:
    AlbumsPage(Core& core, QWidget* parent=nullptr);
    ~AlbumsPage();

private:
    Ui::AlbumsPage* ui;
    Core& core;
};
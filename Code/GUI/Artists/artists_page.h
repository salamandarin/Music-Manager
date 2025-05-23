// Samuel Sutton - 2025
#pragma once
#include <QWidget>

namespace Ui {
    class ArtistsPage;
}
class Core;
class MainWindow;

class ArtistsPage : public QWidget {
    Q_OBJECT
public:
    ArtistsPage(Core& core, MainWindow* parent=nullptr);
    ~ArtistsPage();

    void update_list();
    
private:
    Ui::ArtistsPage* ui;
    Core& core;
};
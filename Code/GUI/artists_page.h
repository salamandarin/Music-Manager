// Samuel Sutton - 2025
#pragma once
#include <QWidget>

namespace Ui {
    class ArtistsPage;
}
class Core;

class ArtistsPage : public QWidget {
    Q_OBJECT

public:
    ArtistsPage(Core& core, QWidget* parent=nullptr);
    ~ArtistsPage();

private:
    void update_list();

    Ui::ArtistsPage* ui;
    Core& core;
};
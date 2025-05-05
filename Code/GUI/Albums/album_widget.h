// Samuel Sutton - 2025
#pragma once
#include <QWidget>

namespace Ui {
    class AlbumWidget;
}
class Album;

class AlbumWidget : public QWidget {
    Q_OBJECT
public:
    AlbumWidget(Album& core, QWidget* parent=nullptr);
    ~AlbumWidget();

signals:
    void album_double_clicked(Album& album);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    void set_data();

    Ui::AlbumWidget* ui;
    Album& album;
};
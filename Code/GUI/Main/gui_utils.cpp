// Samuel Sutton - 2025
#include "gui_utils.h"
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>

namespace gui_utils {

void set_image(std::string image_path, const std::string& default_image, QLabel* label, QWidget* error_popup_parent) {
    // use default image if image_path is empty
    if (image_path.empty()) image_path = default_image;

    // make pixmap
    QPixmap image_pixmap(QString::fromStdString(image_path));
    // handle if failed to load
    if (image_pixmap.isNull()) {
        QMessageBox::warning(error_popup_parent, "Warning", QString::fromStdString("Failed to load image from: " + image_path));
        image_pixmap.load(QString::fromStdString(default_image)); // set to default image
    }
    
    // round corners
    image_pixmap = round_image_corners(image_pixmap, 30);

    // put pixmap in given label
    label->setPixmap(image_pixmap);
    label->setScaledContents(true); // let QLabel scale it
}

QPixmap round_image_corners(const QPixmap& image, int radius) {
    if (image.isNull()) return image; // return if null

    QPixmap rounded(image.size());
    rounded.fill(Qt::transparent);

    QPainter painter(&rounded);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    
    // clip to rounded rectangle
    QPainterPath path;
    path.addRoundedRect(rounded.rect(), radius, radius);
    painter.setClipPath(path);
    
    // draw original image
    painter.drawPixmap(0, 0, image);
    
    return rounded;
}

}
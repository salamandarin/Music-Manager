// Samuel Sutton - 2025
#include "gui_utils.h"
#include <QMessageBox>

void set_label_image(std::string image_path, const std::string& default_image, QLabel* parent_label, QWidget* error_popup_parent) {
    // use default image if image_path is empty
    if (image_path.empty()) image_path = default_image;

    // make pixmap
    QPixmap image_pixmap(QString::fromStdString(image_path));
    // handle if failed to load
    if (image_pixmap.isNull()) {
        QMessageBox::warning(error_popup_parent, "Warning", QString::fromStdString("Failed to load image from: " + image_path));
        image_pixmap.load(QString::fromStdString(default_image)); // set to default image
    }

    // put pixmap in given label
    parent_label->setPixmap(image_pixmap); // set cover art
    parent_label->setScaledContents(true); // let QLabel scale it
}
QLabel* create_image_label(const std::string& image_path, const std::string& default_image, QWidget* parent, QWidget* error_popup_parent) {
    // put image in a QLabel for scaling
    QLabel* image_label = new QLabel(parent);
    set_label_image(image_path, default_image, image_label, error_popup_parent);
    return image_label;
}

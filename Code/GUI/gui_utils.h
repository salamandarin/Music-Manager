// Samuel Sutton - 2025
#pragma once
#include <string>
#include <QWidget>
#include <QLabel>

const std::string DEFAULT_TRACK_IMAGE = "Code/GUI/Default_Images/default_track.jpg";
const std::string DEFAULT_IMAGE = "Code/GUI/Default_Images/default_image.jpg";

void set_label_image(std::string image_path, const std::string& default_image, QLabel* parent_label,
                    QWidget* error_popup_parent=nullptr);
QLabel* create_image_label(const std::string& image_path, const std::string& default_image,
                        QWidget* parent=nullptr, QWidget* error_popup_parent=nullptr);
// TODO: clean up parent pointers?
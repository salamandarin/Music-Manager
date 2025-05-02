// Samuel Sutton - 2025
#pragma once
#include <string>
#include <QWidget>
#include <QLabel>

const std::string DEFAULT_TRACK_IMAGE = "Code/GUI/Default_Images/default_track.jpg";
const std::string DEFAULT_IMAGE = "Code/GUI/Default_Images/default_image.jpg";

void set_image(std::string image_path, const std::string& default_image, QLabel* label,
                QWidget* error_popup_parent); // TODO: delete error_popup_parent???

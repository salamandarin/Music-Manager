// Samuel Sutton - 2025
#include "settings_popup.h"
#include "ui_settings_popup.h"
#include "core.h"

SettingsPopup::SettingsPopup(Core& core, QWidget* parent)
    :core{core}, QDialog{parent}, ui{new Ui::SettingsPopup} {
    
    ui->setupUi(this);
}

SettingsPopup::~SettingsPopup() {
    delete ui;
}

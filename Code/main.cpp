// Samuel Sutton - 2025
#include "core.h"
#include "main_window.h" // TODO: USE
#include "new_main_window.h" // TODO: DELETE
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        QApplication application(argc, argv); // starts Qt program
        Core core;

        NewMainWindow new_main_window(core);
        new_main_window.show();

        return application.exec(); // keeps program running
    }
    catch (std::runtime_error& error) {
        std::cout << "\nError: " << error.what() << "\n";
    }
 }

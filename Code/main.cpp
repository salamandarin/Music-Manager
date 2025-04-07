// Samuel Sutton - 2025
#include "core.h"
#include "main_window.h"
#include <QApplication>
#include <QWidget>
// TODO: DELETE
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
    try {
        QApplication application(argc, argv); // starts Qt program
        Core core;

        MainWindow main_window(core);
        main_window.show();

        return application.exec(); // keeps program running
    }
    catch (std::runtime_error& error) {
        std::cout << "\nError: " << error.what() << "\n";
    }
 }

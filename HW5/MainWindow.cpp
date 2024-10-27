#include "MainWindow.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

enum USERS_SELECT {
    CREATE_FILE = 1,  // 1
    IMPORT_FILE,
    EXPORT_FILE,
    PRINT_FILE,
    SELECT_FIGURE,  // 5
    SELECT_COLOR,
    ADD_FIGURE,
    DELETE_FIGURE,

    EXIT = -1
};

namespace hw5 {

MainWindow::MainWindow() {
    _screen        = std::make_unique<Screen>();
    _toolBar       = std::make_unique<ToolBar>();
    _selectedColor = std::make_unique<Color>();
}

void MainWindow::run() {
    _running = true;
    while (_running) {
        std::cout << "-------------------------------------" << std::endl
                  << "Выберите функцию:" << std::endl
                  << '\t' << CREATE_FILE << ") Создать новый файл" << std::endl
                  << '\t' << IMPORT_FILE << ") Импорт файла" << std::endl
                  << '\t' << EXPORT_FILE << ") Экспорт файла" << std::endl
                  << '\t' << PRINT_FILE << ") Вывод файла" << std::endl
                  << '\t' << SELECT_FIGURE << ") Выбрать фигуру" << std::endl
                  << '\t' << SELECT_COLOR << ") Выбрать цвет" << std::endl
                  << '\t' << ADD_FIGURE << ") Нарисовать фигуру" << std::endl
                  << '\t' << DELETE_FIGURE << ") Удалить фигуру" << std::endl
                  << std::endl
                  << '\t' << EXIT << ") Выйти из программы" << std::endl;

        int choose;
        std::cin >> choose;
        switch (choose) {
        case CREATE_FILE:  // 1
            _createNewDocument();
            break;
        case IMPORT_FILE:  // 2
            _importDocument();
            break;
        case EXPORT_FILE:  // 3
            _exportDocument();
            break;
        case PRINT_FILE:  // 4
            _printDocument();
            break;
        case SELECT_FIGURE:  // 5
            _selectTool();
            break;
        case SELECT_COLOR:  // 6
            _selectColor();
            break;
        case ADD_FIGURE:  // 7
            _addPrimitive();
            break;
        case DELETE_FIGURE:  // 8
            _removePrimitive();
            break;
        case EXIT:  //-1
            std::cout << "Выход из программы..." << std::endl;
            _running = false;
            break;
        default:
            std::cout << "Выбрана неверная опция :(" << std::endl;
        }
    }
}
void MainWindow::_createNewDocument() {
    std::cout << "Введите путь до файла (.txt):" << std::endl;

    std::string path;
    std::cin >> path;
    if (!std::filesystem::exists(path)) {
        std::ofstream f(path);
        f.close();
        _selectedDoc = std::make_shared<Document>(path);
        std::cout << "Файл создан!" << std::endl;
    } else {
        std::cout << "Файл уже существует!" << std::endl;
    }
}

void MainWindow::_importDocument() {
    std::cout << "Введите путь до файла (.txt):" << std::endl;

    std::string path;
    std::cin >> path;
    if (std::filesystem::exists(path)) {
        _selectedDoc = std::make_shared<Document>(path);
        _selectedDoc->parse();
        _printDocument();
        std::cout << "Файл импортирован!" << std::endl;
    } else {
        std::cout << "Файла не существует!" << std::endl;
    }
}

void MainWindow::_exportDocument() {
    std::ofstream fout(_selectedDoc->getPath());
    if (!fout.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        return;
    }

    _screen->printDocument(_selectedDoc.get(), fout);
    fout.close();
    std::cout << "Документ сохранен!" << std::endl;
}

void MainWindow::_selectTool() {
    _toolBar->printPrimitivesList();
    std::cout << std::endl << EXIT << ") Вернуться назад" << std::endl << std::endl;

    int choose;
    std::cin >> choose;
    if (choose == EXIT) {
        return;
    }

    try {
        _selectedTool = _toolBar->select(choose - 1);
        std::cout << "Выбран " << _selectedTool->getName() << std::endl;
    } catch (const std::out_of_range&) {
        std::cout << "Выбран неверный аргумент" << std::endl;
    }
}

void MainWindow::_printDocument() {
    _screen->printDocument(_selectedDoc.get());
}

void MainWindow::_selectColor() {
    std::cout << "Введите компоненты цвета в формате R G B (числа от 0 до 255)" << std::endl;
    int r, g, b;
    std::cin >> r >> g >> b;
    _selectedColor->setComponents(r, g, b);
}

void MainWindow::_addPrimitive() {
    if (!_selectedDoc.get()) {
        std::cout << "Не выбран документ!" << std::endl;
        return;
    }

    if (!_selectedTool.get()) {
        std::cout << "Не выбран примитив!" << std::endl;
        return;
    }

    std::cout << "Введите координаты: x_start, y_start, x_end, y_end" << std::endl;

    int xs, ys, xe, ye;
    std::cin >> xs >> ys >> xe >> ye;

    _selectedTool->setColor(*_selectedColor.get());
    _selectedTool->setCoords(xs, ys, xe, ye);
    _selectedDoc->addPrimitive(std::move(_selectedTool));
    _selectedTool = nullptr;
    _printDocument();
}

void MainWindow::_removePrimitive() {
    if (!_selectedDoc.get()) {
        std::cout << "Не выбран документ!" << std::endl;
        return;
    }

    _printDocument();
    std::cout << "Введите номер элемента для удаления:" << std::endl;

    int index;
    std::cin >> index;

    try {
        _selectedDoc->removePrimetive(index - 1);
    } catch (const std::out_of_range&) {
        std::cout << "Выбран неверный аргумент" << std::endl;
    }
}

}  // namespace hw5

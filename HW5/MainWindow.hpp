#pragma once

#include <memory>
#include <string>

#include "Screen.hpp"
#include "Tools/Primitives/Color.hpp"
#include "Tools/ToolBar.hpp"

namespace hw5 {

class MainWindow {
public:
    MainWindow();

    void run();

private:
    void _createNewDocument();
    void _importDocument();
    void _exportDocument();
    void _printDocument();
    void _selectTool();
    void _selectColor();
    void _addPrimitive();
    void _removePrimitive();

private:
    bool _running = false;
    std::unique_ptr<Screen> _screen;
    std::unique_ptr<ToolBar> _toolBar;

    std::shared_ptr<Document> _selectedDoc    = nullptr;
    std::shared_ptr<IPrimitive> _selectedTool = nullptr;
    std::unique_ptr<Color> _selectedColor;
};

}  // namespace hw5

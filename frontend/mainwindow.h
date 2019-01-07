#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Frontend {
class Equipment;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void makeObjects();

private:
    Ui::MainWindow*                     ui;
    QScopedPointer<Frontend::Equipment> mEquipment;
};

#endif // MAINWINDOW_H

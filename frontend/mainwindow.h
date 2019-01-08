#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Frontend {
class Equipment;
class ClientsTab;
class RentsTab;
} // namespace Frontend

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
    Ui::MainWindow*                      ui;
    QScopedPointer<Frontend::Equipment>  mEquipment;
    QScopedPointer<Frontend::ClientsTab> mClientsTab;
    QScopedPointer<Frontend::RentsTab>   mRentsTab;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Frontend {
class Equipment;
class ClientsTab;
class RentsTab;
} // namespace Frontend

namespace Backend {
class Database;
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
    Ui::MainWindow*                      ui;
    QScopedPointer<Frontend::Equipment>  mEquipment;
    QScopedPointer<Frontend::ClientsTab> mClientsTab;
    QScopedPointer<Frontend::RentsTab>   mRentsTab;
    QScopedPointer<Backend::Database>    mDatabase;
};

#endif // MAINWINDOW_H

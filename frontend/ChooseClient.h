#pragma once

#include "common.h"
#include "ui_chooseclientwindow.h"
#include <QObject>

namespace Backend {
class Database;
} // namespace Backend

namespace Frontend {

class ChooseClient : public QObject
{
    Q_OBJECT

    Ui::ChooseClientWindow* mUi;
    QDialog                 mDialog;
    Backend::Database&      mDatabase;

    //    void fillClientsList(void);
    void printDefaultTable(void);
    void printFiltredClients(const QString& filter);

public:
    explicit ChooseClient(Backend::Database& database);
    void exec();

signals:
    void clientChosed(Common::ClientDetails& client);

public slots:
    void addPressed();
    void cancelPressed();
    void filterChanged(const QString&);
};

} // namespace Frontend

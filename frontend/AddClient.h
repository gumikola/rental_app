#pragma once

#include "common.h"
#include <QObject>
#include <ui_addclient.h>

namespace Backend {
class Database;
} // namespace Backend

namespace Frontend {

class AddClient : public QObject
{
    Q_OBJECT
    Ui::AddClientWindow* mUi;
    QDialog              mDialog;
    Backend::Database&   mDatabase;

    QString getName();
    QString getSurname();
    QString getAddress();

public:
    AddClient(Backend::Database& database);
    void exec();

public slots:
    void addPressed();
    void cancelPressed();

signals:
    void clientAdded(Common::ClientDetails& client);
};

} // namespace Frontend

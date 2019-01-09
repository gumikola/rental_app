#pragma once

#include "common.h"
#include "ui_editclient.h"
#include <QObject>

namespace Backend {
class Database;
} // namespace Backend

namespace Frontend {

class EditClientDetails : public QObject
{
    Q_OBJECT
    Ui::EditClient*              mUi;
    QDialog                      mDialog;
    const Common::ClientDetails& mPreClient;

    QString            getName();
    QString            getSurname();
    QString            getAddress();
    void               fillWindowClientData(const Common::ClientDetails& client);
    Backend::Database& mDatabase;

public:
    explicit EditClientDetails(const Common::ClientDetails& client, Backend::Database& database);
    void exec();
signals:
    void clientChanged(const Common::ClientDetails& client);

public slots:
    void savePressed();
    void cancelPressed();
};

} // namespace Frontend

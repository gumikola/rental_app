#pragma once

#include "common.h"
#include "ui_chooseclientwindow.h"
#include <QObject>

namespace Frontend {

class ChooseClient : public QObject
{
    Q_OBJECT

    Ui::ChooseClientWindow*        mUi;
    QDialog                        mDialog;
    QVector<Common::ClientDetails> mClients;

    void fillClientsList(void);
    void printDefaultTable(void);
    void printFiltredClients(const QString& filter);

public:
    explicit ChooseClient();
    void exec();

signals:
    void clientChosed(Common::ClientDetails& client);

public slots:
    void addPressed();
    void cancelPressed();
    void filterChanged(const QString&);
};

} // namespace Frontend

/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#pragma once

#include <QAbstractListModel>

#include <documentfile.h>

using namespace android;

/*!
 * \brief The DocumentsController is a very simple QML friendly class
 *  needed to perform the most common operations on Android's shared storage.
 */
class DocumentsController : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasSingleUri READ hasSingleUri NOTIFY uriChanged)
    Q_PROPERTY(bool hasTreeUri READ hasTreeUri NOTIFY uriChanged)
    Q_PROPERTY(bool hasParent READ hasParent NOTIFY treeDocChanged)
    Q_PROPERTY(QByteArray fileContent READ fileContent NOTIFY uriChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY uriChanged)
    Q_PROPERTY(QString url READ url NOTIFY uriChanged)

public:
    explicit DocumentsController(QObject *parent = nullptr);
    bool hasSingleUri() const;
    bool hasTreeUri() const;
    bool hasParent() const;
    QByteArray fileContent() const;
    QString fileName() const;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;


    QString url() const;

public slots:
    void newFile(const QString &fileName);
    void openFile(const QStringList &mimeTypes = {"*/*"});
    void openDir();
    void remove();
    void saveContent(const QByteArray &content);

    void cdUp();
    void newTreeFile(const QString &name);
    void newTreeFolder(const QString &name);

    void openTreeItem(int idx);
    void removeTreeItem(int idx);
    void renameTreeItem(int idx, const QString &newName);

signals:
    void uriChanged();
    void treeDocChanged();

private:
    void openTree(const provider::DocumentFilePtr &tree = {});
    void openUri(const net::Uri &uri);

private:
    net::Uri m_uri;
    provider::DocumentFilePtr m_tree;
    std::vector<provider::DocumentFilePtr> m_files;
};

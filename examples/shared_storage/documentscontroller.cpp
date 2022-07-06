/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#include "documentscontroller.h"

#include <contentresolver.h>
#include <documentscontract.h>
#include <sharedstorage.h>

using namespace android::provider;
using namespace android::content;

enum {
    NAME_ROLE = Qt::UserRole + 1,
    TYPE_ROLE,
    SIZE_ROLE,
    MIME_ROLE
};

DocumentsController::DocumentsController(QObject *parent)
    : QAbstractListModel{parent}
{}

bool DocumentsController::hasSingleUri() const
{
    return m_uri.isValid() && DocumentsContract::isDocumentUri(m_uri) && (!m_tree || m_uri != m_tree->uri());
}

bool DocumentsController::hasTreeUri() const
{
    return m_tree.get();
}

bool DocumentsController::hasParent() const
{
    return m_tree && m_tree->parent();
}

QByteArray DocumentsController::fileContent() const
{
    return hasSingleUri() ? ContentResolver::instance().openUri(m_uri, QIODevice::ReadOnly)->readAll()
                          : QByteArray{};
}

void DocumentsController::newFile(const QString &fileName)
{
    SharedStorage::instance().createDocument(1,
        [this](int code, net::Uri uri){
            if (code != 1 || !uri.isValid())
                return;
            ContentResolver::instance().takePersistableRWUriPermissions(uri);
            openTree();
            openUri(uri);
        }, "application/octet-stream", fileName);
}

void DocumentsController::openFile(const QStringList &mimeTypes)
{
    SharedStorage::instance().openDocument(2,
        [this](int code, net::Uri uri){
            if (code != 2 || !uri.isValid())
                return;
            ContentResolver::instance().takePersistableRWUriPermissions(uri);
            openTree();
            openUri(uri);
        }, mimeTypes);
}

void DocumentsController::openDir()
{
    SharedStorage::instance().openDocumentTree(3,
       [this](int code, net::Uri uri){
           if (code != 3 || !uri.isValid())
               return;
           ContentResolver::instance().takePersistableRWUriPermissions(uri);
           openTree(DocumentFile::fromTreeUri(uri));
           openUri(uri);
       });
}

void DocumentsController::remove()
{
    DocumentsContract::removeDocument(m_uri);
    openUri({});
}

void DocumentsController::saveContent(const QByteArray &content)
{
    ContentResolver::instance().openUri(m_uri, QIODevice::WriteOnly | QIODevice::Truncate)->write(content);
    openTree(m_tree);
}

void DocumentsController::cdUp()
{
    if (!m_tree->parent())
        return;
    const auto doc = m_tree->parent();
    openTree(doc);
    openUri(doc->uri());
}

void DocumentsController::newTreeFile(const QString &name)
{
    m_tree->createFile("application/octet-stream", name);
    openTree(m_tree);
}

void DocumentsController::newTreeFolder(const QString &name)
{
    m_tree->createDirectory(name);
    openTree(m_tree);
}

void DocumentsController::openTreeItem(int idx)
{
    if (idx < 0 || idx >= int(m_files.size()))
        return;
    const auto doc = m_files[idx];
    if (doc->isDirectory())
        openTree(doc);
    openUri(doc->uri());
}

void DocumentsController::removeTreeItem(int idx)
{
    if (idx < 0 || idx >= int(m_files.size()))
        return;
    m_files[idx]->remove();
    openTree(m_tree);
    openUri(m_tree->uri());
}

void DocumentsController::renameTreeItem(int idx, const QString &newName)
{
    if (idx < 0 || idx >= int(m_files.size()))
        return;
    m_files[idx]->rename(newName);
    openTree(m_tree);
    openUri(m_tree->uri());
}

void DocumentsController::openTree(const DocumentFilePtr &tree)
{
    beginResetModel();
    if ((m_tree = tree))
        m_files = m_tree->listFiles();
    else
        m_files.clear();
    std::sort(m_files.begin(), m_files.end(),[](const provider::DocumentFilePtr &a, const provider::DocumentFilePtr &b){
        if (a->isDirectory() == b->isDirectory())
            return a->name() < b->name();
        return a->isDirectory();
    });
    endResetModel();
    emit treeDocChanged();
}

void DocumentsController::openUri(const net::Uri &uri)
{
    m_uri = uri;
    emit uriChanged();
}

QString DocumentsController::fileName() const
{
    return m_uri.isValid() ? hasSingleUri() ? DocumentFile::fromSingleUri(m_uri)->name()
                                            : DocumentFile::fromTreeUri(m_uri)->name()
                           : QString{};
}


int DocumentsController::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? -1 : m_files.size();
}

QVariant DocumentsController::data(const QModelIndex &index, int role) const
{
    const int row = index.row();
    if (row < 0 || row >= int(m_files.size()))
        return {};
    switch (role) {
    case NAME_ROLE:
        return m_files[row]->name();
    case SIZE_ROLE:
        return double(m_files[row]->length());
    case MIME_ROLE:
        return m_files[row]->mimeType();
    case TYPE_ROLE:
        return m_files[row]->isDirectory() ? "dir" : "file";
    }
    return {};
}

QHash<int, QByteArray> DocumentsController::roleNames() const
{
    static QHash<int, QByteArray> res = {
        {NAME_ROLE, "name"},
        {TYPE_ROLE, "type"},
        {SIZE_ROLE, "size"},
        {MIME_ROLE, "mime"}
    };
    return res;
}

QString DocumentsController::url() const
{
    return m_uri.isValid() ? m_uri.toString() : QString{};
}

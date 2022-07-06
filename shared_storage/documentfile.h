/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#pragma once

#include "utils.h"
#include <QDateTime>
#include <memory>

namespace android {
namespace provider {

/*!
 *
 * DocumentFile Api.
 * Check https://developer.android.com/reference/androidx/documentfile/provider/DocumentFile
 * for more informations.
 *
 */
using DocumentFilePtr = std::shared_ptr<class DocumentFile>;
class DocumentFile : public std::enable_shared_from_this<DocumentFile>
{
public:
    static DocumentFilePtr fromSingleUri(const net::Uri &uri);
    static DocumentFilePtr fromTreeUri(const net::Uri &treeUri);

    DocumentFilePtr createFile(const QString &mimeType, const QString &displayName);
    DocumentFilePtr createDirectory(const QString &displayName);
    const net::Uri &uri() const;
    const DocumentFilePtr &parent() const;
    QString name() const;
    QString mimeType() const;
    bool isDirectory() const;
    bool isFile() const;
    bool isVirtual() const;
    QDateTime lastModified() const;
    int64_t length() const;
    bool canRead() const;
    bool canWrite() const;
    bool remove();
    bool exists() const;
    std::vector<DocumentFilePtr> listFiles();
    bool rename(const QString &displayName);

protected:
    DocumentFile(const net::Uri &uri, const std::shared_ptr<DocumentFile> &parent);

protected:
    net::Uri m_uri;
    DocumentFilePtr m_parent;
};

} // namespace provider
} // namespace android

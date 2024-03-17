/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#pragma once

#include <QObject>
#include <functional>

#include "utils.h"

namespace android {
namespace provider {

/*!
 *
 * SharedStorage Api.
 *
 */
class SharedStorage : public QObject
{
    Q_OBJECT

    SharedStorage() = default;
public:
    static SharedStorage &instance();
    using RequestResultCallback = std::function<void(int, net::Uri)>;
    using RequestMultipleResultsCallback = std::function<void(int, QList<net::Uri>)>;

    void createDocument(int requestCode, RequestResultCallback callback, const QString &mimeType, const QString &displayName, const QString &initialUri = {});
    void openDocument(int requestCode, const RequestResultCallback &callback, const QStringList &mimeTypes, const QString &initialUri = {});
    void openDocuments(int requestCode, const RequestMultipleResultsCallback &callback, const QStringList &mimeTypes, const QString &initialUri = {});
    void openDocumentTree(int requestCode, const RequestResultCallback &callback, const QString &initialUri = {});

public slots:
    void createDocument(int requestCode, const QString &mimeType, const QString &displayName, const QString &initialUri = {});
    void openDocument(int requestCode, const QStringList &mimeTypes, const QString &initialUri = {});
    void openDocuments(int requestCode, const QStringList &mimeTypes, const QString &initialUri = {});
    void openDocumentTree(int requestCode, const QString &initialUri = {});

signals:
    void requestResult(int requestId, net::Uri uri);
    void requestResults(int requestId, QList<net::Uri> uris);
};

} // namespace provider
} // namespace android

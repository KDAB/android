/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#pragma once

#include <QObject>
#include <QIODevice>
#include <QList>

#include "utils.h"

namespace android {

namespace database {
class Cursor;
}

namespace content {
struct AndroidSyncInfo
{
    AndroidAccount account;
    QString authority;
    qint64 startTime;
};

/*!
 *
 * ContentResolver Api.
 * Check https://developer.android.com/reference/android/content/ContentResolver
 * for more informations.
 *
 */
class ContentResolver
{
    ContentResolver();
public:
    static ContentResolver &instance();
    std::unique_ptr<database::Cursor> queryUri(const net::Uri &url,
                                             const QStringList &projection = {},
                                             const QString &selection = {},
                                             const QStringList &selectionArgs = {},
                                             const QString &sortOrder = {});
    std::unique_ptr<QIODevice> openUri(const net::Uri &url, QIODevice::OpenMode mode);
    int deleteUri(const net::Uri &uri, const QString &where= {}, const QStringList &selectionArgs = {});
    QString getMimeType(const net::Uri &uri);
    void takePersistableRWUriPermissions(const net::Uri &uri);
    bool isSyncAutomatically(const AndroidAccount &account, const QString &authority);
    void setSyncAutomatically(const AndroidAccount &account, const QString &authority, bool on);
    QList<AndroidSyncInfo> getCurrentSyncs();

    jobject object() const;
private:
    QJniObject m_object;
};

} // namespace content
} // namespace android


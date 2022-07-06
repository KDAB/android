/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#include "contentresolver.h"
#include "cursor.h"
#include <QFile>

namespace android {
namespace content {

using namespace utils;

ContentResolver::ContentResolver()
    :  m_object{QJniObject{androidContext()}.callObjectMethod("getContentResolver",
                                                             "()Landroid/content/ContentResolver;")}
{
    Q_ASSERT(m_object.isValid());
}

ContentResolver &ContentResolver::instance()
{
    static ContentResolver res;
    return res;
}

std::unique_ptr<database::Cursor> ContentResolver::queryUri(const net::Uri &url,
                                                          const QStringList &projection,
                                                          const QString &selection,
                                                          const QStringList &selectionArgs,
                                                          const QString &sortOrder)
{
    auto cursor = m_object.callObjectMethod("query",
                                            "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;",
                                            url.object(),
                                            projection.isEmpty() ? nullptr : fromStringList(projection).object(),
                                            selection.isEmpty() ? nullptr : QJniObject::fromString(selection).object(),
                                            selectionArgs.isEmpty() ? nullptr : fromStringList(selectionArgs).object(),
                                            sortOrder.isEmpty() ? nullptr : QJniObject::fromString(sortOrder).object());
    if (!cursor.isValid())
        return {};
    return std::make_unique<database::Cursor>(cursor);
}

std::unique_ptr<QIODevice> ContentResolver::openUri(const net::Uri &url, QIODevice::OpenMode mode)
{
    QString openMode;
    if (mode & QIODevice::OpenModeFlag::ReadOnly)
        openMode += "r";
    if (mode & QIODevice::OpenModeFlag::WriteOnly)
        openMode += "w";
    if (mode & QIODevice::OpenModeFlag::Append)
        openMode += "a";
    if (mode & QIODevice::OpenModeFlag::Truncate)
        openMode += "t";
    auto fileDescriptor = m_object.callObjectMethod("openFileDescriptor",
                                                    "(Landroid/net/Uri;Ljava/lang/String;)Landroid/os/ParcelFileDescriptor;",
                                                    url.object(),
                                                    QJniObject::fromString(openMode).object());
    if (!fileDescriptor.isValid())
        return {};
    auto file = std::make_unique<QFile>();
    if (!file->open(fileDescriptor.callMethod<jint>("detachFd"), mode, QFileDevice::FileHandleFlag::AutoCloseHandle))
        return {};
    return file;
}

int ContentResolver::deleteUri(const net::Uri &uri, const QString &where, const QStringList &selectionArgs)
{
    return m_object.callMethod<jint>("delete",
                                     "(Landroid/net/Uri;Ljava/lang/String;[Ljava/lang/String;)I",
                                     uri.object(),
                                     where.isEmpty() ? nullptr : QJniObject::fromString(where).object(),
                                     selectionArgs.isEmpty() ? nullptr : fromStringList(selectionArgs).object());
}

QString ContentResolver::getMimeType(const net::Uri &uri)
{
    return m_object.callObjectMethod("getType", "(Landroid/net/Uri;)Ljava/lang/String;", uri.object()).toString();
}

void ContentResolver::takePersistableRWUriPermissions(const net::Uri &uri)
{
    m_object.callMethod<void>("takePersistableUriPermission",
                              "(Landroid/net/Uri;I)V",
                              uri.object(),
                              FLAG_GRANT_READ_URI_PERMISSION | FLAG_GRANT_WRITE_URI_PERMISSION);
}

bool ContentResolver::isSyncAutomatically(const AndroidAccount &account, const QString &authority)
{
    return m_object.callMethod<jboolean>("getSyncAutomatically", "(Landroid/accounts/Account;Ljava/lang/String;)Z",
                                         account.object(), QJniObject::fromString(authority).object());
}

void ContentResolver::setSyncAutomatically(const AndroidAccount &account, const QString &authority, bool on)
{
    m_object.callMethod<void>("setSyncAutomatically", "(Landroid/accounts/Account;Ljava/lang/String;Z)V",
                              account.object(), QJniObject::fromString(authority).object(), on);
}

QList<AndroidSyncInfo> ContentResolver::getCurrentSyncs()
{
    QList<AndroidSyncInfo> res;
    auto list = QJniObject::callStaticObjectMethod("android/content/ContentResolver","getCurrentSyncs", "()Ljava/util/List;");
    const int size = list.callMethod<jint>("size");
    for (int i = 0; i < size; ++i) {
        const auto syncInfo = list.callObjectMethod("get", "Ljava/lang/Object;");
        AndroidSyncInfo asi;
        asi.account = syncInfo.getObjectField("account", "Landroid/accounts/Account;");
        asi.authority = syncInfo.getObjectField("authority", "Ljava/lang/String;").toString();
        asi.startTime = syncInfo.getField<jlong>("startTime");
        res << asi;
    }
    return res;
}

jobject ContentResolver::object() const
{
    return m_object.object();
}

} // namespace content
} // namespace android

/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#include "sharedstorage.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
using namespace QtAndroidPrivate;
#else
using namespace QtAndroid;
#endif

namespace {

const QLatin1String ACTION_CREATE_DOCUMENT("android.intent.action.CREATE_DOCUMENT");
const QLatin1String ACTION_OPEN_DOCUMENT("android.intent.action.OPEN_DOCUMENT");
const QLatin1String ACTION_OPEN_DOCUMENT_TREE("android.intent.action.OPEN_DOCUMENT_TREE");
const QLatin1String CATEGORY_OPENABLE("android.intent.category.OPENABLE");
const QLatin1String EXTRA_MIME_TYPES("android.intent.extra.MIME_TYPES");
const QLatin1String EXTRA_ALLOW_MULTIPLE("android.intent.extra.ALLOW_MULTIPLE");
const QLatin1String EXTRA_TITLE("android.intent.extra.TITLE");

constexpr int RESULT_OK = 0xffffffff;
QJniObject createDocument(const QString &mimeType, const QString &title, const QString &initialUri)
{    
    QJniObject intent{"android.content.Intent", "(Ljava/lang/String;)V", QJniObject::fromString(ACTION_CREATE_DOCUMENT).object()};
    intent.callObjectMethod("addCategory", "(Ljava/lang/String;)Landroid/content/Intent;", QJniObject::fromString(CATEGORY_OPENABLE).object());
    intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QJniObject::fromString(mimeType).object());
    intent.callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                            QJniObject::fromString(EXTRA_TITLE).object(),
                            QJniObject::fromString(title).object());
    if (!initialUri.isEmpty()) {
        intent.callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                                QJniObject::getStaticObjectField("android/provider/DocumentsContract", "EXTRA_INITIAL_URI", "Ljava/lang/String;").object(),
                                QJniObject::fromString(initialUri).object());
    }
    return intent;
}

QJniObject openDocument(const QStringList &mimeTypes, const QString &initialUri, bool allowMultiple)
{
    QJniObject intent{"android.content.Intent", "(Ljava/lang/String;)V", QJniObject::fromString(ACTION_OPEN_DOCUMENT).object()};
    intent.callObjectMethod("addCategory", "(Ljava/lang/String;)Landroid/content/Intent;", QJniObject::fromString(CATEGORY_OPENABLE).object());
    intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QJniObject::fromString("*/*").object());
    intent.callObjectMethod("putExtra", "(Ljava/lang/String;[Ljava/lang/String;)Landroid/content/Intent;",
                            QJniObject::fromString(EXTRA_MIME_TYPES).object(),
                            android::utils::fromStringList(mimeTypes).object());
    if (!initialUri.isEmpty()) {
        intent.callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                                QJniObject::getStaticObjectField("android/provider/DocumentsContract", "EXTRA_INITIAL_URI", "Ljava/lang/String;").object(),
                                QJniObject::fromString(initialUri).object());
    }
    if (allowMultiple) {
        intent.callObjectMethod("putExtra", "(Ljava/lang/String;Z)Landroid/content/Intent;",
                                QJniObject::fromString(EXTRA_ALLOW_MULTIPLE).object(),
                                static_cast<jboolean>(true));
    }
    return intent;
}

QJniObject openDocumentTree(const QString &initialUri)
{
    QJniObject intent{"android.content.Intent", "(Ljava/lang/String;)V", QJniObject::fromString(ACTION_OPEN_DOCUMENT_TREE).object()};
    if (!initialUri.isEmpty()) {
        intent.callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                                QJniObject::getStaticObjectField("android/provider/DocumentsContract", "EXTRA_INITIAL_URI", "Ljava/lang/String;").object(),
                                QJniObject::fromString(initialUri).object());
    }
    return intent;
}

QList<android::net::Uri> getUrisFromIntent(const QJniObject &intent) {
    auto uris = QList<android::net::Uri>();
    auto clipData = intent.callObjectMethod("getClipData", "()Landroid/content/ClipData;");
    auto singleUri = intent.callObjectMethod("getData", "()Landroid/net/Uri;");
    if (clipData.isValid()) {
        // User selected multiple files
        auto itemCount = clipData.callMethod<jint>("getItemCount");
        for (int i = 0; i < itemCount; i++) {
            auto item = clipData.callObjectMethod("getItemAt", "(I)Landroid/content/ClipData$Item;", i);
            if (item.isValid()) {
                auto uri = item.callObjectMethod("getUri", "()Landroid/net/Uri;");
                uris.push_back(uri);
            }
        }
    } else if (singleUri.isValid()) {
        // User selected single file
        uris.push_back(singleUri);
    }
    return uris;
}
}

namespace android {
namespace provider {

SharedStorage &SharedStorage::instance()
{
    static SharedStorage res;
    return res;
}

void SharedStorage::createDocument(int requestCode, RequestResultCallback callback, const QString &mimeType, const QString &displayName, const QString &initialUri)
{
    startActivity(::createDocument(mimeType, displayName, initialUri), requestCode,
                  [requestCode, callback, this](int requestId, int resultCode, QJniObject data) {
                      if (requestCode == requestId && resultCode == RESULT_OK)
                          QMetaObject::invokeMethod(this, [=]{callback(requestId, data.callObjectMethod("getData", "()Landroid/net/Uri;"));}, Qt::QueuedConnection);
                  });
}

void SharedStorage::openDocument(int requestCode, const RequestResultCallback &callback, const QStringList &mimeTypes, const QString &initialUri)
{
    startActivity(::openDocument(mimeTypes, initialUri, false /* allowMultiple */), requestCode,
                  [requestCode, callback, this](int requestId, int resultCode, QJniObject data) {
                      if (requestCode == requestId && resultCode == RESULT_OK)
                          QMetaObject::invokeMethod(this, [=]{callback(requestId, data.callObjectMethod("getData", "()Landroid/net/Uri;"));}, Qt::QueuedConnection);
                  });
}

void SharedStorage::openDocuments(int requestCode, const RequestMultipleResultsCallback &callback, const QStringList &mimeTypes, const QString &initialUri)
{
    startActivity(::openDocument(mimeTypes, initialUri, true /* allowMultiple */), requestCode,
                  [requestCode, callback, this](int requestId, int resultCode, QJniObject data) {
                      if (requestCode == requestId && resultCode == RESULT_OK)
                          QMetaObject::invokeMethod(this, [=]{callback(requestId, ::getUrisFromIntent(data));}, Qt::QueuedConnection);
                  });
}

void SharedStorage::openDocumentTree(int requestCode, const RequestResultCallback &callback, const QString &initialUri)
{
    startActivity(::openDocumentTree(initialUri), requestCode,
                  [requestCode, callback, this](int requestId, int resultCode, QJniObject data) {
                      if (requestCode == requestId && resultCode == RESULT_OK)
                          QMetaObject::invokeMethod(this, [=]{callback(requestId, data.callObjectMethod("getData", "()Landroid/net/Uri;"));}, Qt::QueuedConnection);
                  });
}

void SharedStorage::createDocument(int requestCode, const QString &mimeType, const QString &displayName, const QString &initialUri)
{
    startActivity(::createDocument(mimeType, displayName, initialUri), requestCode, [requestCode, this](int requestId, int resultCode, QJniObject data) {
        if (requestCode == requestId && resultCode == RESULT_OK)
            QMetaObject::invokeMethod(this, [=]{emit requestResult(requestId, data.callObjectMethod("getData", "()Landroid/net/Uri;"));});
    });
}

void SharedStorage::openDocument(int requestCode, const QStringList &mimeTypes, const QString &initialUri)
{
    startActivity(::openDocument(mimeTypes, initialUri, false /* allowMultiple */), requestCode, [requestCode, this](int requestId, int resultCode, QJniObject data) {
        if (requestCode == requestId && resultCode == RESULT_OK)
            QMetaObject::invokeMethod(this, [=]{emit requestResult(requestId, data.callObjectMethod("getData", "()Landroid/net/Uri;"));});
    });
}

void SharedStorage::openDocuments(int requestCode, const QStringList &mimeTypes, const QString &initialUri)
{
    startActivity(::openDocument(mimeTypes, initialUri, true /* allowMultiple */), requestCode, [requestCode, this](int requestId, int resultCode, QJniObject data) {
        if (requestCode == requestId && resultCode == RESULT_OK)
            QMetaObject::invokeMethod(this, [=]{emit requestResults(requestId, ::getUrisFromIntent(data));});
    });
}

void SharedStorage::openDocumentTree(int requestCode, const QString &initialUri)
{
    startActivity(::openDocumentTree(initialUri), requestCode, [requestCode, this](int requestId, int resultCode, QJniObject data) {
        if (requestCode == requestId && resultCode == RESULT_OK)
            QMetaObject::invokeMethod(this, [=]{emit requestResult(requestId, data.callObjectMethod("getData", "()Landroid/net/Uri;"));});
    });
}

} // namespace provider
} // namespace android

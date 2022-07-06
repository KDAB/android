/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#include "contentresolver.h"
#include "documentscontract.h"

namespace android {

using namespace content;

namespace provider {
namespace DocumentsContract {

QString documentId(const net::Uri &uri)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "getDocumentId",
                                              "(Landroid/net/Uri;)Ljava/lang/String;",
                                              uri.object()).toString();
}

QString treeDocumentId(const net::Uri &uri)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "getTreeDocumentId",
                                              "(Landroid/net/Uri;)Ljava/lang/String;",
                                              uri.object()).toString();
}

QString rootId(const net::Uri &uri)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "getRootId",
                                              "(Landroid/net/Uri;)Ljava/lang/String;",
                                              uri.object()).toString();
}

net::Uri buildChildDocumentsUriUsingTree(const net::Uri &uri)
{
    return buildChildDocumentsUriUsingTree(uri, treeDocumentId(uri));
}

net::Uri buildChildDocumentsUriUsingTree(const net::Uri &uri, const QString &parentDocumentId)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "buildChildDocumentsUriUsingTree",
                                              "(Landroid/net/Uri;Ljava/lang/String;)Landroid/net/Uri;",
                                              uri.object(),
                                              QJniObject::fromString(parentDocumentId).object());

}


net::Uri buildDocumentUriUsingTree(const net::Uri &treeUri, const QString &documentId)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "buildDocumentUriUsingTree",
                                              "(Landroid/net/Uri;Ljava/lang/String;)Landroid/net/Uri;",
                                              treeUri.object(),
                                              QJniObject::fromString(documentId).object());
}

net::Uri copyDocument(const net::Uri &sourceDocumentUri, const net::Uri &targetParentDocumentUri)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "copyDocument",
                                              "(Landroid/content/ContentResolver;Landroid/net/Uri;Landroid/net/Uri;)Landroid/net/Uri;",
                                              ContentResolver::instance().object(),
                                              sourceDocumentUri.object(),
                                              targetParentDocumentUri.object());
}

net::Uri createDocument(const net::Uri &parentDocumentUri, const QString &mimeType, const QString &displayName)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "createDocument",
                                              "(Landroid/content/ContentResolver;Landroid/net/Uri;Ljava/lang/String;Ljava/lang/String;)Landroid/net/Uri;",
                                              ContentResolver::instance().object(),
                                              parentDocumentUri.object(),
                                              QJniObject::fromString(mimeType).object(),
                                              QJniObject::fromString(displayName).object());
}

bool removeDocument(const net::Uri &documentUri)
{
    return QJniObject::callStaticMethod<jboolean>("android/provider/DocumentsContract",
                                                  "deleteDocument",
                                                  "(Landroid/content/ContentResolver;Landroid/net/Uri;)Z",
                                                  ContentResolver::instance().object(),
                                                  documentUri.object());
}

net::Uri moveDocument(const net::Uri &sourceDocumentUri,
                      const net::Uri &sourceParentDocumentUri,
                      const net::Uri &targetParentDocumentUri)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "moveDocument",
                                              "(Landroid/content/ContentResolver;Landroid/net/Uri;Landroid/net/Uri;Landroid/net/Uri;)Landroid/net/Uri;",
                                              ContentResolver::instance().object(),
                                              sourceDocumentUri.object(),
                                              sourceParentDocumentUri.object(),
                                              targetParentDocumentUri.object());

}

void ejectRoot(const net::Uri &rootUri)
{
    QJniObject::callStaticMethod<void>("android/provider/DocumentsContract",
                                       "ejectRoot",
                                       "(Landroid/content/ContentResolver;Landroid/net/Uri;)V",
                                       ContentResolver::instance().object(),
                                       rootUri.object());
}

bool removeDocument(const net::Uri &documentUri, const net::Uri &parentDocumentUri)
{
    return QJniObject::callStaticMethod<jboolean>("android/provider/DocumentsContract",
                                                  "removeDocument",
                                                  "(Landroid/content/ContentResolver;Landroid/net/Uri;Landroid/net/Uri;)Z",
                                                  ContentResolver::instance().object(),
                                                  documentUri.object(),
                                                  parentDocumentUri.object());
}

net::Uri renameDocument(const net::Uri &documentUri, const QString &displayName)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "renameDocument",
                                              "(Landroid/content/ContentResolver;Landroid/net/Uri;Ljava/lang/String;)Landroid/net/Uri;",
                                              ContentResolver::instance().object(),
                                              documentUri.object(),
                                              QJniObject::fromString(displayName).object());
}

QString searchDocumentsQuery(const net::Uri &uri)
{
    return QJniObject::callStaticObjectMethod("android/provider/DocumentsContract",
                                              "getSearchDocumentsQuery",
                                              "(Landroid/net/Uri;)Ljava/lang/String;",
                                              uri.object()).toString();
}

bool isChildDocument(const net::Uri &parentDocumentUri, const net::Uri &childDocumentUri)
{
    return QJniObject::callStaticMethod<jboolean>("android/provider/DocumentsContract",
                                                  "isChildDocument",
                                                  "(Landroid/content/ContentResolver;Landroid/net/Uri;Landroid/net/Uri;)Z",
                                                  ContentResolver::instance().object(),
                                                  parentDocumentUri.object(),
                                                  childDocumentUri.object());
}

using namespace utils;

bool isDocumentUri(const net::Uri &uri)
{
    return QJniObject::callStaticMethod<jboolean>("android/provider/DocumentsContract",
                                                  "isDocumentUri",
                                                  "(Landroid/content/Context;Landroid/net/Uri;)Z",
                                                  androidContext().object(),
                                                  uri.object());
}

bool isRootUri(const net::Uri &uri)
{
    return QJniObject::callStaticMethod<jboolean>("android/provider/DocumentsContract",
                                                  "isRootUri",
                                                  "(Landroid/content/Context;Landroid/net/Uri;)Z",
                                                  androidContext().object(),
                                                  uri.object());
}

bool isRootsUri(const net::Uri &uri)
{
    return QJniObject::callStaticMethod<jboolean>("android/provider/DocumentsContract",
                                                  "isRootsUri",
                                                  "(Landroid/content/Context;Landroid/net/Uri;)Z",
                                                  androidContext().object(),
                                                  uri.object());
}

bool isTreeUri(const net::Uri &uri)
{
    return QJniObject::callStaticMethod<jboolean>("android/provider/DocumentsContract",
                                                  "isTreeUri",
                                                  "(Landroid/net/Uri;)Z",
                                                  uri.object());
}

} // namespace QAndroidDocumentsContract
} // namespace provider
} // namespace android

/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#include "documentfile.h"

#include "contentresolver.h"
#include "cursor.h"
#include "documentscontract.h"

namespace android {
namespace provider {

using namespace content;
using namespace DocumentsContract;
using namespace utils;
using namespace net;

namespace {
class MakeableDocumentFile : public DocumentFile
{
public:
    MakeableDocumentFile(const Uri &uri, const DocumentFilePtr &parent = {})
        : DocumentFile(uri, parent)
    {}
};

QVariant query(const Uri &uri, const QString &column)
{
    const auto qry = ContentResolver::instance().queryUri(uri, {column});
    if (qry->rowCount() != 1 || qry->columnCount() != 1)
        return {};
    qry->moveToFirst();
    return qry->data(0);
}
}

DocumentFile::DocumentFile(const net::Uri &uri,
                           const DocumentFilePtr &parent)
    : m_uri{uri}
    , m_parent{parent}
{}

DocumentFilePtr DocumentFile::fromSingleUri(const net::Uri &uri)
{
    return std::make_shared<MakeableDocumentFile>(uri);
}

DocumentFilePtr DocumentFile::fromTreeUri(const net::Uri &treeUri)
{
    auto docId = treeDocumentId(treeUri);
    if (isDocumentUri(treeUri))
        docId = documentId(treeUri);
    return std::make_shared<MakeableDocumentFile>(buildDocumentUriUsingTree(treeUri, docId));
}

DocumentFilePtr DocumentFile::createFile(const QString &mimeType, const QString &displayName)
{
    if (isDirectory())
        return std::make_shared<MakeableDocumentFile>(createDocument(m_uri, mimeType, displayName), shared_from_this());
    return {};
}

DocumentFilePtr DocumentFile::createDirectory(const QString &displayName)
{
    if (isDirectory())
        return std::make_shared<MakeableDocumentFile>(createDocument(m_uri, Document::MIME_TYPE_DIR, displayName), shared_from_this());
    return {};
}

const Uri &DocumentFile::uri() const
{
    return m_uri;
}

const DocumentFilePtr &DocumentFile::parent() const
{
    return m_parent;
}

QString DocumentFile::name() const
{
    return query(m_uri, Document::COLUMN_DISPLAY_NAME).toString();
}

QString DocumentFile::mimeType() const
{
    return query(m_uri, Document::COLUMN_MIME_TYPE).toString();
}

bool DocumentFile::isDirectory() const
{
    return mimeType() == Document::MIME_TYPE_DIR;
}

bool DocumentFile::isFile() const
{
    const auto type = mimeType();
    return  type != Document::MIME_TYPE_DIR && !type.isEmpty();
}

bool DocumentFile::isVirtual() const
{
    return isDocumentUri(m_uri) && (query(m_uri, Document::COLUMN_FLAGS).toInt() & Document::FLAG_VIRTUAL_DOCUMENT);
}

QDateTime DocumentFile::lastModified() const
{
    return QDateTime::fromMSecsSinceEpoch(query(m_uri, Document::COLUMN_LAST_MODIFIED).toLongLong());
}

int64_t DocumentFile::length() const
{
    return query(m_uri, Document::COLUMN_SIZE).toLongLong();
}

bool DocumentFile::canRead() const
{
    int perm = androidContext().callMethod<jint>("checkCallingOrSelfUriPermission",
                                                 "(Landroid/net/Uri;I)I",
                                                 m_uri.object(),
                                                 FLAG_GRANT_READ_URI_PERMISSION);
    return perm == 0 && !mimeType().isEmpty();
}

bool DocumentFile::canWrite() const
{
    if (androidContext().callMethod<jint>("checkCallingOrSelfUriPermission",
                                          "(Landroid/net/Uri;I)I",
                                          m_uri.object(),
                                          FLAG_GRANT_WRITE_URI_PERMISSION)) {
        return false;
    }
    const auto type = mimeType();
    if (type.isEmpty())
        return false;

    const int flags = query(m_uri, Document::COLUMN_FLAGS).toInt();

    if (flags & Document::FLAG_SUPPORTS_DELETE)
        return true;

    return (type == Document::MIME_TYPE_DIR && (flags & Document::FLAG_DIR_SUPPORTS_CREATE)) ||
           (flags &  Document::FLAG_SUPPORTS_WRITE);
}

bool DocumentFile::remove()
{
    return removeDocument(m_uri);
}

bool DocumentFile::exists() const
{
    return !name().isEmpty();
}

std::vector<DocumentFilePtr> DocumentFile::listFiles()
{
    std::vector<DocumentFilePtr> res;
    const auto childrenUri = buildChildDocumentsUriUsingTree(m_uri, documentId(m_uri));
    const auto query = ContentResolver::instance().queryUri(childrenUri, {Document::COLUMN_DOCUMENT_ID});
    while (query->moveToNext()) {
        const auto uri = buildDocumentUriUsingTree(m_uri, query->data(0).toString());
        res.push_back(std::make_shared<MakeableDocumentFile>(uri, shared_from_this()));
    }
    return res;
}

bool DocumentFile::rename(const QString &displayName)
{
    auto uri = renameDocument(m_uri, displayName);
    if (uri.isValid()) {
        m_uri = uri;
        return true;
    }
    return false;
}

} // namespace provider
} // namespace android

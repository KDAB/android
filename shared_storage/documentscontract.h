/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#pragma once

#include "utils.h"

namespace android {
namespace provider {

/*!
 *
 * DocumentsContract Api.
 * Check https://developer.android.com/reference/android/provider/DocumentsContract
 * for more informations.
 *
 */
namespace DocumentsContract
{
namespace Document {
const QLatin1String COLUMN_DISPLAY_NAME("_display_name");
const QLatin1String COLUMN_DOCUMENT_ID("document_id");
const QLatin1String COLUMN_FLAGS("flags");
const QLatin1String COLUMN_ICON("icon");
const QLatin1String COLUMN_LAST_MODIFIED("last_modified");
const QLatin1String COLUMN_MIME_TYPE("mime_type");
const QLatin1String COLUMN_SIZE("_size");
const QLatin1String COLUMN_SUMMARY("summary");

constexpr int FLAG_DIR_BLOCKS_OPEN_DOCUMENT_TREE = 0x00008000;
constexpr int FLAG_DIR_PREFERS_GRID = 0x00000010;
constexpr int FLAG_DIR_PREFERS_LAST_MODIFIED = 0x00000020;
constexpr int FLAG_DIR_SUPPORTS_CREATE = 0x00000008;
constexpr int FLAG_PARTIAL = 0x00002000;
constexpr int FLAG_SUPPORTS_COPY = 0x00000080;
constexpr int FLAG_SUPPORTS_DELETE = 0x00000004;
constexpr int FLAG_SUPPORTS_METADATA = 0x00004000;
constexpr int FLAG_SUPPORTS_MOVE = 0x00000100;
constexpr int FLAG_SUPPORTS_REMOVE = 0x00000400;
constexpr int FLAG_SUPPORTS_RENAME = 0x00000040;
constexpr int FLAG_SUPPORTS_SETTINGS = 0x00000800;
constexpr int FLAG_SUPPORTS_THUMBNAIL = 0x00000001;
constexpr int FLAG_SUPPORTS_WRITE = 0x00000002;
constexpr int FLAG_VIRTUAL_DOCUMENT = 0x00000200;
constexpr int FLAG_WEB_LINKABLE = 0x00001000;

const QLatin1String MIME_TYPE_DIR("vnd.android.document/directory");
} // namespace Document

namespace Root {
const QLatin1String COLUMN_AVAILABLE_BYTES("available_bytes");
const QLatin1String COLUMN_CAPACITY_BYTES("capacity_bytes");
const QLatin1String COLUMN_DOCUMENT_ID("document_id");
const QLatin1String COLUMN_FLAGS("flags");
const QLatin1String COLUMN_ICON("icon");
const QLatin1String COLUMN_MIME_TYPES("mime_types");
const QLatin1String COLUMN_QUERY_ARGS("query_args");
const QLatin1String COLUMN_ROOT_ID("root_id");
const QLatin1String COLUMN_SUMMARY("summary");
const QLatin1String COLUMN_TITLE("title");

constexpr int FLAG_EMPTY = 0x00000040;
constexpr int FLAG_LOCAL_ONLY = 0x00000002;
constexpr int FLAG_SUPPORTS_CREATE = 0x00000001;
constexpr int FLAG_SUPPORTS_EJECT = 0x00000020;
constexpr int FLAG_SUPPORTS_IS_CHILD = 0x00000010;
constexpr int FLAG_SUPPORTS_RECENTS = 0x00000004;
constexpr int FLAG_SUPPORTS_SEARCH = 0x00000008;

const QLatin1String MIME_TYPE_ITEM("vnd.android.document/root");
} // namespace Root

net::Uri buildChildDocumentsUriUsingTree(const net::Uri &uri);

net::Uri buildChildDocumentsUriUsingTree(const net::Uri &uri,
                                         const QString &parentDocumentId);

net::Uri buildDocumentUriUsingTree(const net::Uri &treeUri,
                                   const QString &documentId);

net::Uri createDocument (const net::Uri &parentDocumentUri,
                        const QString &mimeType,
                        const QString &displayName);

bool removeDocument(const net::Uri &documentUri);

bool removeDocument (const net::Uri &documentUri,
                    const net::Uri &parentDocumentUri);

net::Uri renameDocument (const net::Uri &documentUri,
                        const QString &displayName);

net::Uri copyDocument(const net::Uri &sourceDocumentUri,
                      const net::Uri &targetParentDocumentUri);

net::Uri moveDocument (const net::Uri &sourceDocumentUri,
                      const net::Uri &sourceParentDocumentUri,
                      const net::Uri &targetParentDocumentUri);


void ejectRoot(const net::Uri &rootUri);

QString documentId(const net::Uri &uri);

QString rootId(const net::Uri &uri);

QString searchDocumentsQuery(const net::Uri &uri);

QString treeDocumentId(const net::Uri &uri);

bool isChildDocument(const net::Uri &parentDocumentUri,
                     const net::Uri &childDocumentUri);

bool isDocumentUri(const net::Uri &uri);

bool isRootUri(const net::Uri &uri);

bool isRootsUri(const net::Uri &uri);

bool isTreeUri(const net::Uri &uri);

};

} // namespace provider
} // namespace android

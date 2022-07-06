/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#include "cursor.h"

namespace android {
namespace database {

namespace {
constexpr int FIELD_TYPE_NULL       = 0x00000000;
constexpr int FIELD_TYPE_INTEGER    = 0x00000001;
constexpr int FIELD_TYPE_FLOAT      = 0x00000002;
constexpr int FIELD_TYPE_STRING     = 0x00000003;
constexpr int FIELD_TYPE_BLOB       = 0x00000004;
}

Cursor::Cursor(const QJniObject &object)
    : m_object{object}
{}

Cursor::~Cursor()
{
    if (m_object.isValid())
        m_object.callMethod<void>("close");
}

QVariant Cursor::data(int columnIndex) const
{
    int type = m_object.callMethod<jint>("getType", "(I)I", columnIndex);
    switch (type) {
    case FIELD_TYPE_NULL:
        return {};
    case FIELD_TYPE_INTEGER:
        return QVariant::fromValue(m_object.callMethod<jlong>("getLong",  "(I)J", columnIndex));
    case FIELD_TYPE_FLOAT:
        return QVariant::fromValue(m_object.callMethod<jdouble>("getDouble",  "(I)D", columnIndex));
    case FIELD_TYPE_STRING:
        return QVariant::fromValue(m_object.callObjectMethod("getString",  "(I)Ljava/lang/String;", columnIndex).toString());
    case FIELD_TYPE_BLOB: {
        auto blob = m_object.callObjectMethod("getBlob",  "(I)[B", columnIndex);
        QJniEnvironment env;
        const auto blobArray = blob.object<jbyteArray>();
        const int size = env->GetArrayLength(blobArray);
        const auto byteArray = env->GetByteArrayElements(blobArray, nullptr);
        QByteArray data{reinterpret_cast<const char *>(byteArray), size};
        env->ReleaseByteArrayElements(blobArray, byteArray, 0);
        return QVariant::fromValue(data);
    }
    }
    return {};
}

int Cursor::columnCount() const
{
    return m_object.callMethod<jint>("getColumnCount");
}

int Cursor::columnIndex(const QString &columnName) const
{
    return m_object.callMethod<jint>("getColumnIndex",
                                     "(Ljava/lang/String;)I",
                                     QJniObject::fromString(columnName).object());
}

QString Cursor::columnName(int columnIndex) const
{
    return m_object.callObjectMethod("getColumnName",
                                     "(I)Ljava/lang/String;",
                                     columnIndex).toString();
}

QStringList Cursor::columnNames() const
{
    QStringList res;
    auto namesObject = m_object.callObjectMethod("getColumnNames",  "()[Ljava/lang/String;");
    QJniEnvironment env;
    const auto array = namesObject.object<jobjectArray>();
    const int size = env->GetArrayLength(array);
    for (int i = 0; i < size; ++i)
        res << QJniObject::fromLocalRef(env->GetObjectArrayElement(array, i)).toString();
    return res;
}

int Cursor::rowCount() const
{
    return m_object.callMethod<jint>("getCount");
}

int Cursor::row() const
{
    return m_object.callMethod<jint>("getPosition");
}

bool Cursor::isAfterLast() const
{
    return m_object.callMethod<jboolean>("isAfterLast");
}

bool Cursor::isBeforeFirst() const
{
    return m_object.callMethod<jboolean>("isBeforeFirst");
}

bool Cursor::isFirst() const
{
    return m_object.callMethod<jboolean>("isFirst");
}

bool Cursor::isLast() const
{
    return m_object.callMethod<jboolean>("isLast");
}

bool Cursor::isNull(int columnIndex) const
{
    return m_object.callMethod<jboolean>("isNull", "(I)Z", columnIndex);
}

bool Cursor::move(int offset)
{
    return m_object.callMethod<jboolean>("move", "(I)Z", offset);
}

bool Cursor::moveToFirst()
{
    return m_object.callMethod<jboolean>("moveToFirst");
}

bool Cursor::moveToLast()
{
    return m_object.callMethod<jboolean>("moveToLast");
}

bool Cursor::moveToNext()
{
    return m_object.callMethod<jboolean>("moveToNext");
}

bool Cursor::moveToPosition(int position)
{
    return m_object.callMethod<jboolean>("moveToPosition", "(I)Z", position);
}

bool Cursor::moveToPrevious()
{
    return m_object.callMethod<jboolean>("moveToPrevious");

}

} // namespace database
} // namespace android

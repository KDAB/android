/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#pragma once

#include <QAbstractListModel>
#include <QObject>

#include "utils.h"

namespace android {
namespace database {
/*!
 *
 * Cursor Api.
 * Check https://developer.android.com/reference/android/database/Cursor
 * for more informations.
 *
 */
class Cursor
{
public:
    explicit Cursor(const QJniObject &object);
    ~Cursor();

    QVariant data(int columnIndex) const;
    int columnCount() const;
    int columnIndex(const QString &columnName) const;
    QString columnName(int columnIndex) const;
    QStringList columnNames() const;
    int rowCount() const;
    int row() const;
    bool isAfterLast() const;
    bool isBeforeFirst() const;
    bool isFirst() const;
    bool isLast() const;
    bool isNull(int columnIndex) const;
    bool move(int offset);
    bool moveToFirst();
    bool moveToLast();
    bool moveToNext();
    bool moveToPosition(int position);
    bool moveToPrevious();

private:
    QJniObject m_object;
    QVector<int> m_types;
};

} // namespace database
} // namespace android

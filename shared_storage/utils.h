/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#pragma once

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#  include <private/qandroidextras_p.h>
#  include <private/qjnihelpers_p.h>
#else
#  include <QtAndroid>
#  include <QtAndroidExtras>
using QJniObject  = QAndroidJniObject;
using QJniEnvironment = QAndroidJniEnvironment;
#endif

namespace android {


namespace net {
using Uri = QJniObject;
}

using AndroidAccount = QJniObject;

namespace utils
{
constexpr int FLAG_GRANT_READ_URI_PERMISSION = 0x00000001;
constexpr int FLAG_GRANT_WRITE_URI_PERMISSION = 0x00000002;
net::Uri parseAndroidUri(const QString &url);
QJniObject fromStringList(const QStringList &list);
QJniObject androidContext();
QString guessContentTypeFromName(const QString &name);

} // namespace Utils

} // namespace android

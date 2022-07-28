/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
#include "utils.h"

namespace android {
namespace utils {

QJniObject fromStringList(const QStringList &list)
{
    QJniEnvironment env;
    auto array = env->NewObjectArray(list.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
    for (int i = 0; i < list.size(); ++i)
        env->SetObjectArrayElement(array, i, QJniObject::fromString(list[i]).object());
    return QJniObject::fromLocalRef(array);
}

net::Uri parseAndroidUri(const QString &url)
{
    return QJniObject::callStaticObjectMethod("android/net/Uri", "parse",
                                              "(Ljava/lang/String;)Landroid/net/Uri;",
                                              QJniObject::fromString(url).object());
}

QJniObject androidContext()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return QtAndroidPrivate::context();
#else
    return QtAndroid::androidContext();
#endif
}

QString guessContentTypeFromName(const QString &name)
{
    return QJniObject::callStaticObjectMethod("java/net/URLConnection",
                                              "guessContentTypeFromName",
                                              "(Ljava/lang/String;)Ljava/lang/String;",
                                              QJniObject::fromString(name).object()).toString();
}

} // namespace utils
} // namespace android

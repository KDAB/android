#include "qandroidmediaplayer.h"
#include "qsurfacetexture.h"

#include <QAndroidJniEnvironment>

QAndroidMediaPlayer::QAndroidMediaPlayer(QObject *parent)
    : QObject(parent)
    , m_mediaPlayer("android/media/MediaPlayer")
{
}

QAndroidMediaPlayer::~QAndroidMediaPlayer()
{
    QAndroidJniEnvironment env;
    m_mediaPlayer.callMethod<void>("stop");
    m_mediaPlayer.callMethod<void>("reset");
    m_mediaPlayer.callMethod<void>("release");
}

QSurfaceTexture *QAndroidMediaPlayer::videoOut() const
{
    return m_videoOut;
}

void QAndroidMediaPlayer::setVideoOut(QSurfaceTexture *videoOut)
{
    if (m_videoOut == videoOut)
        return;
    m_videoOut = videoOut;

    // Create a new Surface object from our SurfaceTexture
    QAndroidJniObject surface("android/view/Surface",
                              "(Landroid/graphics/SurfaceTexture;)V",
                               videoOut->surfaceTexture().object());

    // Set the new surface to m_mediaPlayer object
    m_mediaPlayer.callMethod<void>("setSurface", "(Landroid/view/Surface;)V",
                                   surface.object());

    emit videoOutChanged();
}

void QAndroidMediaPlayer::playFile(const QString &file)
{
    QAndroidJniEnvironment env;
    m_mediaPlayer.callMethod<void>("stop"); // try to stop the media player.
    m_mediaPlayer.callMethod<void>("reset"); // try to reset the media player.

    // http://developer.android.com/reference/android/media/MediaPlayer.html#setDataSource(java.lang.String) - the path of the file, or the http/rtsp URL of the stream you want to play.
    m_mediaPlayer.callMethod<void>("setDataSource", "(Ljava/lang/String;)V", QAndroidJniObject::fromString(file).object());

    // prepare media player
    m_mediaPlayer.callMethod<void>("prepare");

    // start playing
    m_mediaPlayer.callMethod<void>("start");
}

#ifndef QSURFACETEXTURE_H
#define QSURFACETEXTURE_H

#include <QAndroidJniObject>
#include <QQuickItem>

class QSurfaceTexture : public QQuickItem
{
    Q_OBJECT
public:
    QSurfaceTexture(QQuickItem *parent = nullptr);
    ~QSurfaceTexture();

    // returns surfaceTexture Java object.
    const QAndroidJniObject &surfaceTexture() const { return m_surfaceTexture; }

    // QQuickItem interface
protected:
    QSGNode *updatePaintNode(QSGNode *n, UpdatePaintNodeData *) override;

private:
    // our texture
    uint32_t m_textureId = 0;

    // Java SurfaceTexture object
    QAndroidJniObject m_surfaceTexture;
};

#endif // QSURFACETEXTURE_H

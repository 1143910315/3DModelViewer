#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QWidget>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DCore/QTransform>
#include <Qt3DWindow>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTorusMesh>

#include <QKeyEvent>
#include <QTransform>
#include <QMesh>
#include <QObjectPicker>
#include <QPainter>
#include <QMainWindow>

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(Qt3DExtras::Qt3DWindow *, QObject *parent=nullptr);

    void NewScene(Qt3DRender::QMesh *);
    void KeyControls(QKeyEvent *event);
    QVector3D mapTo3D(const QPoint &pos);
    void mousePressEvenbt(QMouseEvent *event);
    void moudePress(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void createXian();


public slots:
    void LightXChanged(int angle);
    void LightYChanged(int angle);
    void LightZChanged(int angle);

private:
    Qt3DCore::QEntity *rootEntity;
    //Qt3DCore::QEntity *model;
    QList<Qt3DCore::QEntity *>m_model;
    QList<Qt3DRender::QObjectPicker *>m_picker;
    Qt3DCore::QTransform *transform;
    QList<Qt3DCore::QTransform *>m_transformer;
    Qt3DCore::QTransform *selectTransform;
    Qt3DRender::QDirectionalLight *light;
    Qt3DExtras::Qt3DWindow *window;
    Qt3DRender::QCamera *camera;

    QPoint m_laseMousePos;
    float w_x;//世界坐标
    float w_y;
    float w_z;
    QVector3D point1;
    QVector3D point2;

    void StartScene();
    void Camera(Qt3DExtras::Qt3DWindow *view);
    void SetupMesh(Qt3DCore::QEntity *model,Qt3DRender::QMesh *mesh);
    void SetupTransform(Qt3DCore::QEntity *model);
    void SetupMaterial(Qt3DCore::QEntity *model);
    void SetupLighting();

    float test=0;
};

#endif // SCENE_H

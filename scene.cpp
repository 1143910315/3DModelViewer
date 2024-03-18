#include "scene.h"

#include <QBuffer>
#include <QFirstPersonCameraController>
#include <QObjectPicker>
#include <QViewport>
#include <qcylindermesh.h>
#include <QtMath>
/*顶层实体通常指的是在一个场景或层次结构中处于最高级别的实体或对象。
 在3D图形编程中，顶层实体是整个场景的根节点，它通常包含了场景中的
 所有其他对象、实体或元素。对于3D图形场景，顶层实体是
 整个场景的入口点，所有的渲染、变换和交互都是从这个顶层实体开始的。*/

Scene::Scene(Qt3DExtras::Qt3DWindow *view, QObject *parent) : QObject (parent)
{
    //创建一个根实体，作为整个3D场景的顶层实体，用于包含和组织场景对象中的所有元素和对象
    rootEntity = new Qt3DCore::QEntity();
    //作为传入的QT3DWindow对象的根实体，场景的整体元素都将包含在rootEntity中进行渲染
    //所有的3D对象、光源、相机等都将作为rootEntity的子级，从而构建整个
    //3D场景的层次结构，通过这种方式，可以方便的管理和操作整个3D场景中的各个元素
    view->setRootEntity(rootEntity);
    //设置视颜色背景
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x42f4bc)));
    //设置相机
    Camera(view);
    window = view;
    //初始化场景
    StartScene();

}
/*相机的作用是捕捉场景的图像，将其投影到屏幕上，从而创建出我们所看到的图形。
 通过控制相机的位置、方向和视场等属性，可以改变观察者所看到的场景，
 从而实现不同的视角和观察效果。在代码中，设置相机可以帮助我们确定场景的观察点，
 以便正确呈现出整个场景或特定部分的图像。*/
void Scene::Camera(Qt3DExtras::Qt3DWindow *view)
{
    //获取相机对象
    camera = view->camera();
    //设置相机的透视投影参数，包括视场角、宽高比、近裁剪面和远裁剪面
    //相机可视范围的角度大小、渲染视图的宽度和高度之比、相机到场景的最近可视距离
    //相机到场景的最远可视距禭
    camera->lens()->setPerspectiveProjection(60.0f,16.0f/9.0f, 0.1f, 10000.0f);
    //设置相机位置
    camera->setPosition(QVector3D(0, 0, 90));
    //设置相机的观察中心为场景的中心点
    camera->setViewCenter(QVector3D(0, 0, 0));
}

void Scene::StartScene()
{
    //创建一个新的Qt3DCore::QEntity对象，并将其设置为rootEntity的子级
    Qt3DCore::QEntity *model = new Qt3DCore::QEntity(rootEntity);

    // 创建一个Qt3DExtras::QTorusMesh对象torusMesh，并将其设置为model的子级
    Qt3DExtras::QTorusMesh *torusMesh = new Qt3DExtras::QTorusMesh();
    //设置torusMesh的半径、次半径、环的数量和切片的数量
    torusMesh->setRadius(5);
    torusMesh->setMinorRadius(1);
    torusMesh->setRings(100);
    torusMesh->setSlices(20);
    //将torusMesh作为组件添加到model中
    model->addComponent(torusMesh);
    //调用SetupTransform函数，用于设置模型的变换
    SetupTransform(model);
    // 调用SetupMaterial函数，用于设置模型的材质
    SetupMaterial(model);
    // 调用SetupLighting函数，用于设置模型的光照
    SetupLighting();
    m_model.append(model);
    Qt3DRender::QObjectPicker *picker = new Qt3DRender::QObjectPicker(model);
    picker->setHoverEnabled(true); // 如果需要的话
    model->addComponent(picker);
    m_picker.append(picker);
}

void Scene::NewScene(Qt3DRender::QMesh *mesh)
{
    //delete model;
    Qt3DCore::QEntity *model= new Qt3DCore::QEntity(rootEntity);
    SetupMesh(model,mesh);
    SetupTransform(model);
    SetupMaterial(model);
    SetupLighting();
    m_model.append(model);
    Qt3DRender::QObjectPicker *picker = new Qt3DRender::QObjectPicker(model);
    picker->setHoverEnabled(true);
    model->addComponent(picker);
    m_picker.append(picker);
}

void Scene::SetupMesh(Qt3DCore::QEntity *model,Qt3DRender::QMesh *mesh)
{
    model->addComponent(mesh);
}
float i = -300;
void Scene::SetupTransform(Qt3DCore::QEntity *model)
{
    if(m_transformer.size() == 0)
    {
        transform = new Qt3DCore::QTransform();
        m_transformer.append(transform);
        transform->setScale3D(QVector3D(1, 1, 1));
        transform->setScale(2.0f);
        qDebug()<<"size:"<<m_transformer.size();
        transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 25.0f));
        transform->setTranslation(QVector3D(0, -50, -200));
        qDebug()<<transform->translation().x()<<" "<<transform->translation().y();
        model->addComponent(transform);
    }
    else
    {

            transform = new Qt3DCore::QTransform();
            m_transformer.append(transform);
            transform->setScale3D(QVector3D(1, 1, 1));
            transform->setScale(2.0f);
            qDebug()<<"size:"<<m_transformer.size();
            //transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 25.0f));
            transform->setTranslation(QVector3D(i, 2, -200));
            qDebug()<<transform->translation().x()<<" "<<transform->translation().y();
            model->addComponent(transform);
            i = i+200;

    }
}

void Scene::SetupMaterial(Qt3DCore::QEntity *model)
{
    Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(model);
    model->addComponent(material);
}

void Scene::SetupLighting()
{
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QDirectionalLight*light = new Qt3DRender::QDirectionalLight(lightEntity);
    light->setColor("blue");
    light->setIntensity(1);
    light->setWorldDirection(QVector3D(0,0,-1));

    lightEntity->addComponent(light);
}

void Scene::KeyControls(QKeyEvent *event){
    for(int i = 0;i<m_model.size();i++)
    {
        connect(m_picker[i],&Qt3DRender::QObjectPicker::clicked,[=](){
            transform = m_transformer[i];
        });
    }

    if (event->modifiers().testFlag(Qt::ControlModifier)){
        if(event->key()==Qt::Key_Up){
            transform->setRotationX(transform->rotationX()-6);
        }
        if(event->key()==Qt::Key_Down){
            transform->setRotationX(transform->rotationX()+6);
        }
        if(event->key()==Qt::Key_Left){
            transform->setRotationY(transform->rotationY()-6);
        }
        if(event->key()==Qt::Key_Right){
            transform->setRotationY(transform->rotationY()+6);
        }
    }else if (event->modifiers().testFlag(Qt::ShiftModifier)) {
        if(event->key()==Qt::Key_Up){
            transform->setTranslation(QVector3D(transform->translation().x(),transform->translation().y(),transform->translation().z()-1));
            qDebug()<<transform->translation().z();
        }
        if(event->key()==Qt::Key_Down){
            transform->setTranslation(QVector3D(transform->translation().x(),transform->translation().y(),transform->translation().z()+1));
            qDebug()<<transform->translation().z();
        }
    }else{
        if(event->key()==Qt::Key_Up){
            transform->setTranslation(QVector3D(transform->translation().x(),transform->translation().y()+1,transform->translation().z()));
        }
        if(event->key()==Qt::Key_Down){
            transform->setTranslation(QVector3D(transform->translation().x(),transform->translation().y()-1,transform->translation().z()));
        }
        if(event->key()==Qt::Key_Left){
            transform->setTranslation(QVector3D(transform->translation().x()-1,transform->translation().y(),transform->translation().z()));
        }
        if(event->key()==Qt::Key_Right){
            transform->setTranslation(QVector3D(transform->translation().x()+1,transform->translation().y(),transform->translation().z()));
        }
    }
}

void Scene::LightXChanged(int angle)
{
    light->setWorldDirection(QVector3D(angle,light->worldDirection().y(),light->worldDirection().z()));
}

void Scene::LightYChanged(int angle)
{
    light->setWorldDirection(QVector3D(light->worldDirection().x(),angle,light->worldDirection().z()));
}

void Scene::LightZChanged(int angle)
{
    light->setWorldDirection(QVector3D(light->worldDirection().x(),light->worldDirection().y(),angle));
}


QVector3D Scene::mapTo3D(const QPoint &pos)
{
    QSize viewSize = window->size();
    QMatrix4x4 invertedViewProjectionMatrix = (camera->projectionMatrix() * camera->viewMatrix()).inverted();
    QVector4D viewport((float)pos.x() / viewSize.width(), (float)(viewSize.height() - pos.y()) / viewSize.height(), 0.0, 1.0);
    QVector4D worldPos = invertedViewProjectionMatrix * viewport;
    return QVector3D(worldPos.x() / worldPos.w(), worldPos.y() / worldPos.w(), worldPos.z() / worldPos.w());
}


void Scene::mousePressEvenbt(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint clickPos = event->pos();

        // 将鼠标点击位置转换为三维坐标
        QVector3D worldPos = mapTo3D(clickPos);
        w_x = worldPos.x();
        w_y = worldPos.y();
        w_z = worldPos.z();
        qDebug()<<"world"<<worldPos;

    }
}

void Scene::moudePress(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        // 在这里捕获鼠标按下事件，并记录下初始的鼠标坐标
        m_laseMousePos = event->pos();
        qDebug()<<event->pos().x()<<" "<<event->pos().y()<<event->pos();
    }
    if(event->button() == Qt::RightButton)
    {
        createXian();
    }
}

void Scene::mouseMove(QMouseEvent *event)
{
    for(int i = 0;i<m_model.size();i++)
    {
        connect(m_picker[i],&Qt3DRender::QObjectPicker::clicked,[=](){
            transform = m_transformer[i];
        });
    }
    if(event->buttons()&Qt::LeftButton)
    {
        QPoint delta = event->pos() - m_laseMousePos;

        // 根据差值更新模型的位置
        QVector3D translation = transform->translation();
        translation.setX(translation.x() + delta.x() * 0.06f);
        translation.setY(translation.y() - delta.y() * 0.06f);
        transform->setTranslation(translation);
        qDebug()<<"坐标"<<transform->translation();
        m_laseMousePos = event->pos();
    }
}

void Scene::createXian()
{
    //创建直线实体
    //怎么算，比如我想算这几点
    float x1=test++;
    float y1=0;
    float z1=-1.5;
    float x2=0.0;
    float y2=-4.0;
    float z2=-1.5;

    QVector3D point1(x1, y1, z1);//这是坐标，随表找的点，但是并不显示
    QVector3D point2(x2, y2, z2);
    Qt3DCore::QEntity *lineEntity = new Qt3DCore::QEntity(rootEntity);
    //创建网格组件
    Qt3DExtras::QCylinderMesh *cylinderMesh = new Qt3DExtras::QCylinderMesh();
    cylinderMesh->setRadius(0.1f); // 设置直线半径
    cylinderMesh->setLength(point1.distanceToPoint(point2)); // 设置直线长度

    // 创建材质组件
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    // 创建变换组件并设置位置和方向
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation((point1 + point2) / 2.0f); // 设置直线位置






    // 计算两点之间的向量
    QVector3D direction = point2 - point1;

    // 计算旋转轴，即从 p1 到 p2 的向量
    QVector3D rotationAxis = direction.normalized();

    // 计算旋转角度，即向量与 X 轴之间的夹角
    double rotationAngleX = acos(rotationAxis.x());
    double rotationAngleY = acos(rotationAxis.y());
    double rotationAngleZ = acos(rotationAxis.z());
    if(rotationAxis.y()<0){
        rotationAngleX=-rotationAngleX;
    }


    // 设置旋转
    //transform->setRotationX(rotationAngleX);
    //transform->setRotationZ(rotationAngleX);

    qDebug()<<rotationAngleX<< " "<<rotationAngleX*180.0f/3.14f;
    transform->setRotationZ(rotationAngleX*180.0f/3.14f+90);
    //QQuaternion rotation = QQuaternion::fromAxisAndAngle(rotationAxis, 45.0f);
//QQuaternion rotation = QQuaternion::fromAxisAndAngle(rotationAxis, rotationAngleX*180.0f);
//transform->setRotation(rotation);
    //transform->setRotation(QQuaternion(1, QVector3D(1,0, 0)));
    //QVector3D rotationAxis2 = QVector3D(0, 0, 1); // Z 轴
    //double rotationAngle2 = 45; // 90 度
    // 使用fromAxisAndAngle方法创建旋转四元数
    //QQuaternion rotation = QQuaternion::fromAxisAndAngle(rotationAxis2, rotationAngle2);
    //transform->setRotation(rotation);



    //transform->setRotationZ(30.0f);
    //transform->setRotation(QQuaternion::rotationTo(point1 , point2));
    // transform->setRotation(QQuaternion::rotationTo(QVector3D(1.0f, 0.0f, 0.0f), (point1 - point2))); // 设置直线方向
    // 将网格、材质和变换组件添加到实体中
    lineEntity->addComponent(cylinderMesh);
    lineEntity->addComponent(material);
    lineEntity->addComponent(transform);

    qDebug()<<"point1:"<<point1.x()<<" "<<point1.y()<<" "<<point1.z();
    qDebug()<<"point2:"<<point2.x()<<" "<<point2.y()<<" "<<point2.z();

}



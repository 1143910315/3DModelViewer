 #include "modelviewer.h"
#include "ui_modelviewer.h"
#include "scene.h"

ModelViewer::ModelViewer(QWidget *parent) :
    QMainWindow (parent),
    ui(new Ui::ModelViewer)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::TabFocus);

    //central widget
    QWidget *central = new QWidget();
    setCentralWidget(central);//主窗口的中央部件

    // //scene
    //创建一个用于显示3D内容的窗口类
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    // //将 Qt3DWindow 嵌入到普通的 QWidget 中，以便在界面上显示和布局
    QWidget *sceneWidget = QWidget::createWindowContainer(view);
    scene = new Scene(view);
    // //将当前的 ModelViewer 对象设置为 view 的事件过滤器。这意味着 ModelViewer 对象将能够处理 view 接收到的事件，如键盘事件等。
    view-> installEventFilter(this);

    QHBoxLayout *sceneLayout = new QHBoxLayout;
    sceneLayout->addWidget(sceneWidget);
    // sceneLayout->addWidget(xWidget);
    // sceneLayout->addWidget(yWidget);
    // sceneLayout->addWidget(zWidget);
    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(sceneLayout);
    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(mainWidget);
    central->setLayout(topLayout);
}

QSlider *ModelViewer::createSlider()
{
}

ModelViewer::~ModelViewer()
{
    delete ui;
}
//事件过滤函数声明，用于处理过滤和处理特定类型的事件
bool ModelViewer::eventFilter(QObject *obj, QEvent *event)
{
    //检查事件是否为按键事件
    if (event->type() == QEvent::KeyPress){
        //将事件转换为键盘事件，以便获取键盘按键的详细信息
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //调用Scene类的函数，将键盘事件传递给场景进行处理，这意味这键盘事件的处理逻辑由场景对象
        //的KeyControls函数来执行
        scene->KeyControls(keyEvent);
    }
    if(event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        scene->mousePressEvenbt(mouseEvent);
    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        scene->moudePress(mouseEvent);
    }
    else if(event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        scene->mouseMove(mouseEvent);
    }
    //如果事件不是键盘按键事件，或者在处理完键盘事件后，
    //将事件传递给父类的事件过滤器。
    return QObject::eventFilter(obj, event);
}
//槽函数
void ModelViewer::on_actionOpen_File_triggered()
{
    QString filename= QFileDialog::getOpenFileName(this,"Open file");
    QFile file(filename);
    currentFile = filename;
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,"Warning", "Cannot open "+file.errorString());
    }
    setWindowTitle(filename);
    //用于表示3D模型
    Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();
    //创建一个 QUrl 对象，将文件路径转换为 QUrl 形式。
    QUrl data =QUrl::fromLocalFile(filename);
    //将 QMesh 对象的数据源设置为选定的文件路径。
    mesh->setSource(data);
    //调用 Scene 类的 NewScene 函数，将新创建的 QMesh 对象传递给场景，以便在场景中展示这个3D模型。
    scene->NewScene(mesh);
}

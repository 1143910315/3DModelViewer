#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QMainWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DWindow>

#include <QKeyEvent>

#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QMesh>
#include <QUrl>

#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include <scene.h>

namespace Ui {
class ModelViewer;
}

class ModelViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModelViewer(QWidget *parent = nullptr);
    ~ModelViewer();

    void SetLayout();

protected:
    bool eventFilter(QObject *obj, QEvent *event);


signals:
    void KeyPressed();

private slots:
    void on_actionOpen_File_triggered();
protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);

        QPoint startPoint(50, 100); // 起点坐标
        QPoint endPoint(350, 200); // 终点坐标
        painter.drawLine(startPoint, endPoint);
    }
private:
    Ui::ModelViewer *ui;

    QSlider *createSlider();
    Scene *scene;

    QString currentFile = "";

};

#endif // MODELVIEWER_H

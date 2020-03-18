//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class QMenu;
class QSlider;
class QScrollArea;

class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void renderIntoPixmap();
    void grabFrameBuffer();
    void clearPixmap();
    void about();

private:

    void createMenus();
    void createActions();

    QSlider *createSlider(const char *changedSignal, const char *setterSlot);
    void setPixmap(const QPixmap &pixmap);
    QSize getSize();

    QWidget *centralWidget;
    QScrollArea *glWidgetArea;
    QScrollArea *pixmapLabelArea;
    GLWidget *glWidget;
    QLabel *pixmapLabel;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *renderIntoPixmapAction;
    QAction *grabFrameBufferAction;
    QAction *clearPixmapAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *aboutQtAction;

};

#endif // MAINWINDOW_H

//mainwindow.cpp
#include <QtOpenGL>       // !!!




#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QSlider>
#include <QScrollArea>
#include <QMenuBar>
#include <QApplication>


#include "mainwindow.h"
#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    glWidget = new GLWidget;
    pixmapLabel = new QLabel;

    glWidgetArea = new QScrollArea;
    glWidgetArea->setWidget(glWidget);
    //glWidgetArea->viewport()->setBackgroundRole(QPalette::Dark);
    glWidgetArea->setWidgetResizable(true);
    glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    glWidgetArea->setMinimumSize(50, 50);

    pixmapLabelArea = new QScrollArea;
    pixmapLabelArea->setWidget(pixmapLabel);
    pixmapLabelArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    pixmapLabelArea->setMinimumSize(50, 50);

    //在构造一个QSlider时将QGLWidget的信号和槽传给这个函数的形参，这样就可以在QMainWindow中
    //控制OpenGL的动作了，而让GLWidget类只完成绘图工作。
    xSlider = createSlider(SIGNAL(xRotationChanged(int)),
                           SLOT(setXRotation(int)));
    ySlider = createSlider(SIGNAL(yRotationChanged(int)),
                           SLOT(setYRotation(int)));
    zSlider = createSlider(SIGNAL(zRotationChanged(int)),
                           SLOT(setZRotation(int)));

    /*
    xSlider = new QSlider(Qt::Horizontal);
    ySlider = new QSlider(Qt::Horizontal);
    zSlider = new QSlider(Qt::Horizontal);
    */

    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(glWidgetArea, 0, 0);
    centralLayout->addWidget(pixmapLabelArea, 0, 1);
    centralLayout->addWidget(xSlider, 1, 0, 1, 2);
    centralLayout->addWidget(ySlider, 2, 0, 1, 2);
    centralLayout->addWidget(zSlider, 3, 0, 1, 2);
    centralWidget->setLayout(centralLayout);

    createActions();
    createMenus();

    xSlider->setValue(15 * 16);
    ySlider->setValue(345 * 16);
    zSlider->setValue(0 * 16);

    setWindowTitle(tr("Grabeer"));
    resize(480, 360);
}



void MainWindow::setPixmap(const QPixmap &pixmap)
{
    pixmapLabel->setPixmap(pixmap);
    QSize size = pixmap.size();
    if (size - QSize(1, 0) == pixmapLabelArea->maximumViewportSize())
        size -= QSize(1, 0);
    pixmapLabel->resize(size);
}



QSize MainWindow::getSize()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Grabber"),
                                         tr("Enter Pixmap Size:"),
                                         QLineEdit::Normal,
                                         tr("%1 x %2").arg(glWidget->width())
                                                      .arg(glWidget->height()),
                                         &ok);
    if (!ok)
        return QSize();

    QRegExp regExp(tr("([0-9]+) *x *([0-9]+)"));
    if (regExp.exactMatch(text)) {
        int width = regExp.cap(1).toInt();
        int height = regExp.cap(2).toInt();
        if (width > 0 && width < 2048 && height > 0 && height < 2048)
             return QSize(width, height);
    }
    return glWidget->size();
}



void MainWindow::renderIntoPixmap()
{
    QSize size = getSize();
    if (size.isValid()) {
        QPixmap pixmap = glWidget->renderPixmap(size.width(), size.height());
        setPixmap(pixmap);
    }
}



void MainWindow::grabFrameBuffer()
{
    //QGLWidget有一个返回其帧缓冲区的QImage图片的函数
    QImage image = glWidget->grabFrameBuffer();
    //QPixmap的fromImage函数把一个QImage转换成QPixmap
    setPixmap(QPixmap::fromImage(image));
}



void MainWindow::clearPixmap()
{
    setPixmap(QPixmap()); //给它传一个空的对象
}



void MainWindow::about()
{
    QMessageBox::about(this, tr("About Grabber"),
             tr("The <b>Grabber</b> example demonstrates two approaches for "
                "rendering OpenGL into a Qt pixmap."));
}



QSlider *MainWindow::createSlider(const char *changedSignal,
                                  const char *setterSlot)
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 16 * 360);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);

    //这种经典的用法一定要小心，报错：glWidget的槽函数在传进来的时候已经被强制转换成SLOT了，
    //所以setterSlot不用SLOT修饰；同样，changedSignal也不能再拿SIGNAL修饰
    connect(slider, SIGNAL(valueChanged(int)), glWidget, setterSlot);
    connect(glWidget, changedSignal, slider, SLOT(setValue(int)));

    return slider;
}



void MainWindow::createActions()
{
    renderIntoPixmapAction = new QAction(tr("&Render into Pixmap..."), this);
    renderIntoPixmapAction->setShortcut(tr("Ctrl+R"));
    renderIntoPixmapAction->setToolTip(tr("yes, triggerd it"));
    connect(renderIntoPixmapAction, SIGNAL(triggered()),
            this, SLOT(renderIntoPixmap()));

    grabFrameBufferAction = new QAction(tr("&Grab Frame Buffer"), this);
    grabFrameBufferAction->setShortcut(tr("Ctrl+G"));
    connect(grabFrameBufferAction, SIGNAL(triggered()),
            this, SLOT(grabFrameBuffer()));

    clearPixmapAction = new QAction(tr("&Clear Pixmap"), this);
    clearPixmapAction->setShortcut(tr("Ctrl+L"));
    connect(clearPixmapAction, SIGNAL(triggered()), this, SLOT(clearPixmap()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}



void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(renderIntoPixmapAction);
    fileMenu->addAction(grabFrameBufferAction);
    fileMenu->addAction(clearPixmapAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}



MainWindow::~MainWindow()
{
}

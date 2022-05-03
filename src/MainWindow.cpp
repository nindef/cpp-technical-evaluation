#include "MainWindow.h"
#include "MotionRecorderManager.h"
#include "MotionRecorderWidget.h"

#include <QCoreApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QDialog>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MotionRecorder");
    setCentralWidget(buildCentralWidget ());

    setAttribute(Qt::WA_DeleteOnClose, true);
}

QWidget* MainWindow::buildCentralWidget ()
{
    const auto centralWidget = new QWidget (this);
    mGlobalLayout = new QVBoxLayout;

    const auto configButton = new QPushButton("Setup");
    connect (configButton, &QPushButton::clicked, this, &MainWindow::onConfigButtonClicked);

    mGlobalLayout->addWidget(configButton);
    mGlobalLayout->setAlignment(configButton, Qt::AlignLeft | Qt::AlignTop);

    centralWidget->setLayout(mGlobalLayout);

    return centralWidget;
}

void MainWindow::onConfigButtonClicked()
{
    QDialog configDialog;
    configDialog.setWindowTitle("MotionRecorder Setup");

    const auto layout = new QGridLayout;

    const auto firstLabel = new QLabel ("Record");
    const auto spinBox = new QSpinBox;
    spinBox->setValue(2);
    spinBox->setSuffix(" seconds");
    spinBox->setRange(0, 30);
    const auto secondLabel = new QLabel ("after motion finishes");

    const auto acceptButton = new QPushButton ("Set");
    const auto cancelButton = new QPushButton ("Cancel");
    const auto buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(acceptButton);

    connect (cancelButton, &QPushButton::clicked, &configDialog, &QDialog::reject);
    connect (acceptButton, &QPushButton::clicked, &configDialog, &QDialog::accept);

    layout->addWidget(firstLabel, 0,0);
    layout->addWidget(spinBox, 0,1);
    layout->addWidget(secondLabel, 0,2);
    layout->addItem(new QSpacerItem (0, 10, QSizePolicy::Fixed, QSizePolicy::Fixed), 1, 0);
    layout->addLayout(buttonsLayout, 2, 0, 1, 3);

    configDialog.setLayout(layout);

    if (configDialog.exec() == QDialog::Accepted)
    {
        emit secondsAfterMotionFinishesChanged (spinBox->value());
    }
}

void MainWindow::addMotionRecorder(std::shared_ptr<MotionRecorderManager> motionDetection)
{
    const auto camWidget = new MotionRecorderWidget(motionDetection, this);
    connect (this, &MainWindow::secondsAfterMotionFinishesChanged,
             camWidget, &MotionRecorderWidget::onSecondsAfterMotionFinishesChanged);
    camWidget->setParent(this);
    mGlobalLayout->addWidget(camWidget);

    adjustSize();
}


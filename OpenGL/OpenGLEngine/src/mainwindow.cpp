#include "mainwindow.h"
#include "RayDialog.h"
#include "VectorClass.h"
#include "../GeneratedFiles/ui_mainwindow.h"
#include "../GeneratedFiles/ui_raydialog.h"
#include <QFileDialog>
#include <QListWidgetItem>
#include "GraphicsClass.h"
#include "RayTracer.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	ui = new Ui::MainWindow();
	rayDialog = 0;

	animationTimer = NULL;

	ui->setupUi(this);

	// we create a simple timer, with the widget being its parent
	animationTimer = new QTimer(this);
	// the timer will send a signal timeout at regular intervals.
	// whenever this timeout signal occurs, we want it to call our drawOpenGL
	// function
	connect(animationTimer, SIGNAL(timeout()), this, SLOT(drawOpenGL()));
	// we start the timer with a timeout interval of 20ms
	animationTimer->start(20);

	connect(ui->oglwidget, SIGNAL(addedNewGeometry(std::string)), this, SLOT(newGeometryAdded(std::string)));
	connect(ui->oglwidget, SIGNAL(removedGeometry(std::string)), this, SLOT(geometryRemoved(std::string)));

	connect(ui->addModel, SIGNAL(clicked()), this, SLOT(addGeometry()));
	connect(ui->removeModel, SIGNAL(clicked()), this, SLOT(removeGeometry()));

	connect(ui->XRotSlider, SIGNAL(valueChanged(int)), this, SLOT(rotateGeometryX(int)));
	connect(ui->YRotSlider, SIGNAL(valueChanged(int)), this, SLOT(rotateGeometryY(int)));
	connect(ui->ZRotSlider, SIGNAL(valueChanged(int)), this, SLOT(rotateGeometryZ(int)));

	connect(ui->XTransSlider, SIGNAL(valueChanged(int)), this, SLOT(translateGeometryX(int)));
	connect(ui->YTransSlider, SIGNAL(valueChanged(int)), this, SLOT(translateGeometryY(int)));
	connect(ui->ZTransSlider, SIGNAL(valueChanged(int)), this, SLOT(translateGeometryZ(int)));

	connect(ui->orthoProj, SIGNAL(stateChanged(int)), this, SLOT(setOrtho(int)));
	connect(ui->postMult, SIGNAL(stateChanged(int)), this, SLOT(setPost(int)));

	connect(ui->traceButton, SIGNAL(clicked()), this, SLOT(addTraceDialog()));
}

void MainWindow::drawOpenGL()
{
	// whenever drawOpenGL gets called, we only want to repaint our opengl widget
	ui->oglwidget->repaint();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::newGeometryAdded(std::string m_GeometryName)
{
	ui->modelList->addItem(QString(m_GeometryName.c_str()));
}

void MainWindow::geometryRemoved(std::string m_GeometryName)
{
	QString geometryName(m_GeometryName.c_str());
	QList<QListWidgetItem *> items = ui->modelList->findItems(geometryName, Qt::MatchFixedString);

	QList<QListWidgetItem *>::iterator it = items.begin();

	while(it != items.end())
	{
		delete ui->modelList->takeItem(ui->modelList->row((*it)));
		++it;
	}
}

void MainWindow::addGeometry()
{
	this->on_actionAdd_Model_triggered();
}

void MainWindow::removeGeometry()
{
	this->on_actionRemove_Selected_triggered();
}

void MainWindow::on_actionAdd_Model_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"",
		tr("Files (*.obj)"));

	if(!fileName.isNull())
		ui->oglwidget->loadGeometry(fileName.toStdString());
}

void MainWindow::on_actionRemove_Selected_triggered()
{
	QList<QListWidgetItem *> items = ui->modelList->selectedItems();
	QList<QListWidgetItem *>::iterator it = items.begin();

	while(it != items.end())
	{
		ui->oglwidget->removeGeometry((*it)->text().toStdString());
		++it;
	}
}

void MainWindow::scaleGeometryX(int sliderValue)
{
	Vector3 scaleVec = Vector3();
	scaleVec.getElements()[0] = ((float)sliderValue)/100.0f;

	ui->oglwidget->scaleGeometries(scaleVec);
}

void MainWindow::scaleGeometryY(int sliderValue)
{
	Vector3 scaleVec = Vector3();
	scaleVec.getElements()[1] = ((float)sliderValue)/100.0f;

	ui->oglwidget->scaleGeometries(scaleVec);
}

void MainWindow::scaleGeometryZ(int sliderValue)
{
	Vector3 scaleVec = Vector3();
	scaleVec.getElements()[2] = ((float)sliderValue)/100.0f;

	ui->oglwidget->scaleGeometries(scaleVec);
}

void MainWindow::rotateGeometryX(int sliderValue)
{
	Vector3 scaleVec = Vector3();
	scaleVec.getElements()[0] = ((float)sliderValue)/100.0f;

	ui->oglwidget->rotateGeometries(scaleVec);
}

void MainWindow::rotateGeometryY(int sliderValue)
{
	Vector3 scaleVec = Vector3();
	scaleVec.getElements()[1] = ((float)sliderValue)/100.0f;

	ui->oglwidget->rotateGeometries(scaleVec);
}

void MainWindow::rotateGeometryZ(int sliderValue)
{
	Vector3 scaleVec = Vector3();
	scaleVec.getElements()[2] = ((float)sliderValue)/100.0f;

	ui->oglwidget->rotateGeometries(scaleVec);
}

void MainWindow::translateGeometryX(int sliderValue)
{
	Vector3 scaleVec = Vector3();
	scaleVec.getElements()[0] = ((float)sliderValue)/100.0f;

	ui->oglwidget->translateGeometries(scaleVec);
}

void MainWindow::translateGeometryY(int sliderValue)
{
	Vector3 scaleVec = Vector3();
	scaleVec.getElements()[1] = ((float)sliderValue)/100.0f;

	ui->oglwidget->translateGeometries(scaleVec);
}

void MainWindow::translateGeometryZ(int sliderValue)
{
	Vector3 scaleVec = Vector3();
	scaleVec.getElements()[2] = ((float)sliderValue)/100.0f;

	ui->oglwidget->translateGeometries(scaleVec);
}

void MainWindow::setOrtho(int state)
{
	bool result = false;

	if(state == 0)
		result = false;
	else if(state == 2)
		result = true;

	ui->oglwidget->setOrtho(result);
}

void MainWindow::setPost(int state)
{
	bool result = false;

	if(state == 0)
		result = false;
	else if(state == 2)
		result = true;

	ui->oglwidget->setPost(result);
}

void MainWindow::addTraceDialog()
{
	if(!rayDialog)
	{
		rayDialog = new RayDialog();
	}

	ui->oglwidget->getGraphics()->rayTraceEvent();

	rayDialog->addTraceImage();
}
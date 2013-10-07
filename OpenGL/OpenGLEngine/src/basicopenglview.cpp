#include "basicopenglview.h"
#include "GraphicsClass.h"
#include "ThreeDGraphicsClass.h"
#include "ModelClass.h"
#include "VectorClass.h"
#include <QMessageBox>
#include <QtCore>
#include <iostream>
#include <assert.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include <QMouseEvent>

BasicOpenGLView::BasicOpenGLView(QWidget *parent) :
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#ifdef __APPLE__
	QGLWidget(new Core3_2_context(QGLFormat::defaultFormat()), parent),
#else
	QGLWidget(QGLFormat::defaultFormat(), parent)
#endif
#endif
{
	bool result;

	mGraphicsSystem = new GraphicsClass();

	mLastMousePos = new Vector3();
}

void BasicOpenGLView::loadGeometry(string m_ModelPath)
{
	mGraphicsSystem->get3DGraphics()->addNewModel(m_ModelPath, "./data/stone.tga", "./data/stoneNormal.tga");

	ModelClass* newModel = mGraphicsSystem->get3DGraphics()->getModel(m_ModelPath);
	if(!newModel)
		return;

	emit addedNewGeometry(newModel->getName());
}

void BasicOpenGLView::removeGeometry(string m_ModelPath)
{
	mGraphicsSystem->get3DGraphics()->removeModel(m_ModelPath);

	emit removedGeometry(m_ModelPath);
}

void BasicOpenGLView::scaleGeometries(Vector3 scaleVector)
{
	mGraphicsSystem->get3DGraphics()->scaleAll(scaleVector);
}

void BasicOpenGLView::rotateGeometries(Vector3 rotVector)
{
	mGraphicsSystem->get3DGraphics()->rotateAll(rotVector);
}

void BasicOpenGLView::translateGeometries(Vector3 transVector)
{
	mGraphicsSystem->get3DGraphics()->translateAll(transVector);
}

void BasicOpenGLView::setOrtho(bool val)
{
	mGraphicsSystem->get3DGraphics()->setOrtho(val);
}

void BasicOpenGLView::setPost(bool val)
{
	mGraphicsSystem->get3DGraphics()->setPost(val);
}

void BasicOpenGLView::initializeGL()
{
	bool result;

#ifndef __APPLE__
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Glew could not initialize" << std::endl;
	}
#endif
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);
	
	// Set the polygon winding to front facing for the left handed system.
	glFrontFace(GL_CW);

	// Enable back face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
	std::cout<<glGetString(GL_VERSION)<<std::endl;
#else
	if(this->format().majorVersion() < 3 && this->format().minorVersion() < 2)
	{
		QMessageBox msgBox;
		msgBox.setText("The OpenGL version available on this computer is too old.");
		msgBox.setInformativeText("OpenGL 3.2 or higher required to run this application.");
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Close);
		msgBox.setDefaultButton(QMessageBox::Close);
		msgBox.exec();
		QCoreApplication::quit();
	}
#endif

	result = mGraphicsSystem->Initialize();
	if(!result)
	{
		assert(0);
	}
}

void BasicOpenGLView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mGraphicsSystem->Render();
}

void BasicOpenGLView::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}

void BasicOpenGLView::mousePressEvent(QMouseEvent *event)
{
	// TODO deal with mouse buttons
	mFirstFrameDragging = true;
}

void BasicOpenGLView::mouseReleaseEvent(QMouseEvent *event)
{
	mFirstFrameDragging = true;
}

void BasicOpenGLView::mouseMoveEvent(QMouseEvent *event)
{
	float xTotal, yTotal, distance;
	/**
	*  @todo    assignment 1
	*  convert the mouse event coordinate into OpenGL view coordinates.
	*  store them in curPoint.
	*/
	int mouseX = event->pos().x();
	int mouseY = event->pos().y();

	Vector3 curPoint;
	curPoint.x = mouseX;
	curPoint.y = mouseY;
	curPoint.z = 1;

	// to avoid jumping when a new drag started, we check if this is our first dragging frame
	// in that case we only store our current mouse position
	if(mFirstFrameDragging)
	{
		mLastMousePos = &curPoint;
		mFirstFrameDragging = false;
		return;
	}

	/**
	*  @todo   assignment 1
	*  calculate the difference the mouse has moved in OpenGL coordinates between this frame and
	*  the previous.
	*  translate all geometries accordingly
	*/
	xTotal = curPoint.x-mLastMousePos->x;
	yTotal = curPoint.y-mLastMousePos->y;

	Vector3 translation;
	translation.x = (xTotal / SCREEN_WIDTH)*2;
	translation.y = (yTotal / SCREEN_HEIGHT)*-2;
	translation.z = 0;

	mGraphicsSystem->get3DGraphics()->translateAll(translation);

	mLastMousePos = &curPoint;
}

GraphicsClass* BasicOpenGLView::getGraphics()
{
	return mGraphicsSystem;
}
#include "raydialog.h"
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <qplugin.h>
#include <QPictureFormatPlugin>

//Q_IMPORT_PLUGIN( QJpegPlugin );

RayDialog::RayDialog(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	scene = new QGraphicsScene( this->graphicsView );
}

RayDialog::~RayDialog()
{

}

void RayDialog::addTraceImage()
{
	this->show();
	QFile file("tracedImage.ppm");
	if(file.exists())
	{
		QPixmap image;
		image.load("tracedImage.ppm");
		if(image.isNull())
		{
			return;
		}
		QGraphicsPixmapItem *item_p1 = scene->addPixmap(image);
		item_p1->setVisible(true);
		this->graphicsView->setScene( scene );
		this->graphicsView->show();

	}
}
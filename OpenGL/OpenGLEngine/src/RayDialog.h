#ifndef RAYDIALOG_H
#define RAYDIALOG_H

#include <QWidget>
#include "ui_raydialog.h"

class RayDialog : public QWidget, public Ui::RayDialog
{
	Q_OBJECT

public:
	RayDialog(QWidget *parent = 0);
	~RayDialog();

	void addTraceImage();

private:
	QGraphicsScene *scene;
	QGraphicsPixmapItem *item;
};

#endif // RAYDIALOG_H

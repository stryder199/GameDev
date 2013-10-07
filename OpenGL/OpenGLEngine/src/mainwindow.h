#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
	class MainWindow;
}

class RayDialog;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	public slots:

		/**
		* we use this slot to trigger our widget's redraw function
		* with the timer
		*/
		void drawOpenGL();

		/**
		* @brief newGeometryAdded
		* slot called when a new geometry is added to the scene
		* @param m_GeometryName the name of the geometry
		*/
		void newGeometryAdded(std::string m_GeometryName);

		/**
		* @brief geometryRemoved
		* slot called when a geometry object is removed from the scene
		* @param m_GeometryName
		*/
		void geometryRemoved(std::string m_GeometryName);

		/**
		* @brief addGeometry
		* slot to be called for adding new geometry
		*/
		void addGeometry();

		/**
		* @brief removeGeometry
		* slot to be called for removing geometry
		*/
		void removeGeometry();

		private slots:
			void on_actionAdd_Model_triggered();

			void on_actionRemove_Selected_triggered();

			void scaleGeometryX(int sliderValue);
			void scaleGeometryY(int sliderValue);
			void scaleGeometryZ(int sliderValue);

			void rotateGeometryX(int sliderValue);
			void rotateGeometryY(int sliderValue);
			void rotateGeometryZ(int sliderValue);

			void translateGeometryX(int sliderValue);
			void translateGeometryY(int sliderValue);
			void translateGeometryZ(int sliderValue);

			void setOrtho(int state);
			void setPost(int state);

			void addTraceDialog();

private:
	Ui::MainWindow *ui;
	RayDialog* rayDialog;

	QTimer * animationTimer;        /**< this is the only member so far added to the auto-generated mainwindow class. We use the timer for regular OpenGL redraws. */
};

#endif // MAINWINDOW_H

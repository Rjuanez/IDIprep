#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "./Model/model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
	Q_OBJECT

	public:
		MyGLWidget (QWidget *parent=0);
		~MyGLWidget ();
	protected:
		// initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
		virtual void initializeGL ( );
		// paintGL - Mètode cridat cada cop que cal refrescar la finestra.
		// Tot el que es dibuixa es dibuixa aqui.
		virtual void paintGL ( );
		// resizeGL - És cridat quan canvia la mida del widget
		virtual void resizeGL (int width, int height);
		// keyPressEvent - Es cridat quan es prem una tecla
		virtual void keyPressEvent (QKeyEvent *event);
        virtual void mouseMoveEvent (QMouseEvent *event);

	private:
		// functions
		void createBuffers ();
		void carregaShaders ();
		void patricioTransform ();
        void terraTransform ();
		void ini_camera ();
		void projectTransform ();
		void viewTransform ();
        void calcularParametresEscena(glm::vec3 puntMaxim, glm::vec3 puntMinim);
        void calcularParametresPatricio();
		// attribute locations
		GLuint vertexLoc, colorLoc;
		// uniform locations
		GLuint transLoc, projLoc, viewLoc;
		// VAO names
		GLuint VAO_Patricio, VBO_PatricioPos, VBO_PatricioCol;
		//VBO names
		GLuint VAO_Terra, VBO_TerraPos, VBO_TerraCol;
		// Program
		QOpenGLShaderProgram *program;
		// Internal vars
		Model patricio;
        float scale, rotation;
		glm::vec3 pos;
        glm::vec3 centreEscena;
        float radiEscena, alfaIni;
        glm::vec3 basePatricio;
    float alturaPatricio;
    float angleX = 0 , angleY = 0;
    int pastX, pastY;
		
		// Added vars
		GLfloat FOV;
		GLfloat ra;
		GLfloat znear;
		GLfloat zfar;
		
		glm::vec3 OBS;
		glm::vec3 VRP;
		glm::vec3 UP;

};


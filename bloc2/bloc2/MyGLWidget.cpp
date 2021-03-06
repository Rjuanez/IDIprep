#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
    rotation = 0.0f;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    glEnable(GL_DEPTH_TEST);
    carregaShaders();
    createBuffers();
    ini_camera ();
}

void MyGLWidget::ini_camera ()
{
    calcularParametresEscena(glm::vec3(-2.5,0,-2.5), glm::vec3(2.5,4,2.5));
    
    float diametre = radiEscena * 1.5;
    alfaIni = asin(radiEscena/diametre);
	FOV = 2 * alfaIni;
    FOVini = FOV;
	ra = 1.0;
	znear = diametre-radiEscena;
    zfar = diametre+radiEscena;
    projectTransform();
    
	OBS = centreEscena + diametre*glm::vec3(0,0,1);
	VRP = centreEscena;
	UP = glm::vec3(0,1,0);	  
	viewTransform();
    left = -radiEscena;
    right = radiEscena;
    top = radiEscena;
    bottom = - radiEscena;
  
}

void MyGLWidget::paintGL () 
{
	glViewport (0, 0, width(), height()); // Aquesta crida no caldria perquè Qt la fa de forma automàtica amb aquests paràmetres

	// Esborrem el frame-buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Carreguem la transformació de model
	patricioTransform1();
	glBindVertexArray (VAO_Patricio);
	glDrawArrays(GL_TRIANGLES, 0, patricio.faces().size()*3);
    
    patricioTransform2();
	glDrawArrays(GL_TRIANGLES, 0, patricio.faces().size()*3);
    
    patricioTransform3();
	glDrawArrays(GL_TRIANGLES, 0, patricio.faces().size()*3);
    
    terraTransform ();
    
    // Activem el VAO per a pintar la caseta
    glBindVertexArray (VAO_Terra);

    // pintem
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glBindVertexArray (0);
}

void MyGLWidget::patricioTransform1()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, float(M_PI/2)+rotation, glm::vec3(0,1,0) );
    transform = glm::scale(transform, glm::vec3(1.0/alturaPatricio,1.0/alturaPatricio,1.0/alturaPatricio));
  transform = glm::translate(transform, -basePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
void MyGLWidget::patricioTransform2()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(-2, 0, -2));
    transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, float(M_PI)+rotation, glm::vec3(0,1,0) );
    transform = glm::scale(transform, glm::vec3(1.0/alturaPatricio,1.0/alturaPatricio,1.0/alturaPatricio));
  transform = glm::translate(transform, -basePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
void MyGLWidget::patricioTransform3()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(2, 0, 2));
    transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, rotation, glm::vec3(0,1,0) );
    transform = glm::scale(transform, glm::vec3(1.0/alturaPatricio,1.0/alturaPatricio,1.0/alturaPatricio));
  transform = glm::translate(transform, -basePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
void MyGLWidget::terraTransform ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform () 
{
    glm::mat4 Proj (1.0f);
    if (prespectiva) Proj = glm::perspective (FOV, ra, znear, zfar);
    else Proj = glm::ortho (left, right, bottom, top, znear, zfar);
	glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
	//glm::mat4 View = glm::lookAt (OBS, VRP, UP);
    glm::mat4 View (1.0f);
    View = glm::translate(View, glm::vec3(0, 0, -radiEscena*1.5));
    View = glm::rotate(View, 0.0f+angleY, glm::vec3(1,0,0));
    View = glm::rotate(View, 0.0f+angleX, glm::vec3(0,1,0));
    View = glm::translate(View, -VRP);
	glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
	// Aquí anirà el codi que cal fer quan es redimensiona la finestra
    float rv = float(w)/float(h);
    
    ra = rv;
    if (rv < 1) {
        FOV = 2.0 * atan(tan(alfaIni)/rv);
        top = radiEscena / rv;
        bottom = -radiEscena / rv;
        
    }
    else {
        FOV = 2*alfaIni;
        left = -radiEscena * rv;
        right = radiEscena * rv;
    }
    
    projectTransform();
    
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
	makeCurrent();
	switch (event->key()) 
	{
		case Qt::Key_S: { // escalar a més gran
			scale += 0.05;
			break;
		}
		case Qt::Key_D: { // escalar a més petit
			scale -= 0.05;
			break;
		}
        case Qt::Key_R: { //
            rotation += M_PI/4;
            break;
        }
        case Qt::Key_Z: { // escalar a més petit
            FOV -= 0.01;
            
            alfaIni = FOV / 2;
            projectTransform();
			break;
		}
        case Qt::Key_X: { //
            FOV += 0.01;
            alfaIni = FOV / 2;
            projectTransform();
            break;
        }
        case Qt::Key_O: { //
            prespectiva = !prespectiva;
            projectTransform();
            break;
        }
		default: 
			event->ignore(); 
			break;
	}
	update();

}

void MyGLWidget::mouseMoveEvent (QMouseEvent *e)
{
    
    makeCurrent();
    //if (e->buttons() == Qt::LeftButton && !(e->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier)))
    //{
    float tempX = float(e->x() - pastX);
    float tempY = float(e->y() - pastY);
        if ( tempX < 15 and tempX > -15) angleX += tempX/80.0;
        if ( tempY < 15 and tempY > -15) angleY += tempY/80.0;
        
            std::cout << e->x() - pastX << std::endl;
        pastX = e->x();
        pastY = e->y();
        
        emit changeDialValueX(int(angleX/0.1));
        emit changeDialValueY(int(angleY/0.1));

   
        
        viewTransform ();
        update();
        
        
    //}
}

void MyGLWidget::createBuffers () 
{
	// Dades de la caseta
	// Dos VBOs, un amb posició i l'altre amb color

	// Creació del Vertex Array Object per pintar
	patricio.load("./models/Patricio.obj");
    calcularParametresPatricio();

	glGenVertexArrays(1, &VAO_Patricio);
	glBindVertexArray(VAO_Patricio);

	glGenBuffers(1, &VBO_PatricioPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patricio.faces().size()*3*3, patricio.VBO_vertices(), GL_STATIC_DRAW);

	// Activem l'atribut vertexLoc
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLoc);

	glGenBuffers(1, &VBO_PatricioCol);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioCol);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*patricio.faces().size()*3*3, patricio.VBO_matdiff(), GL_STATIC_DRAW);

	// Activem l'atribut colorLoc
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorLoc);

	// Dades del terra
	// Dos VBOs, un amb posició i l'altre amb color
	glm::vec3 posterra[4] = {
		glm::vec3(-2.5,  0.0, -2.5),
        glm::vec3(-2.5,  0.0,  2.5),
        glm::vec3( 2.5,  0.0, -2.5),
        glm::vec3( 2.5,  0.0,  2.5)
	}; 
	glm::vec3 colterra[4] = {
		glm::vec3(1,0,1),
		glm::vec3(1,0,1),
		glm::vec3(1,0,1),
		glm::vec3(1,0,1)
	};

	// Creació del Vertex Array Object per pintar
	glGenVertexArrays(1, &VAO_Terra);
	glBindVertexArray(VAO_Terra);

	glGenBuffers(1, &VBO_TerraPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

	// Activem l'atribut vertexLoc
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLoc);

	glGenBuffers(1, &VBO_TerraCol);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

	// Activem l'atribut colorLoc
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorLoc);

	glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
	// Creem els shaders per al fragment shader i el vertex shader
	QOpenGLShader fs (QOpenGLShader::Fragment, this);
	QOpenGLShader vs (QOpenGLShader::Vertex, this);
	// Carreguem el codi dels fitxers i els compilem
	fs.compileSourceFile("shaders/fragshad.frag");
	vs.compileSourceFile("shaders/vertshad.vert");
	// Creem el program
	program = new QOpenGLShaderProgram(this);
	// Li afegim els shaders corresponents
	program->addShader(&fs);
	program->addShader(&vs);
	// Linkem el program
	program->link();
	// Indiquem que aquest és el program que volem usar
	program->bind();

	// Obtenim identificador per a l'atribut “vertex” del vertex shader
	vertexLoc = glGetAttribLocation (program->programId(), "vertex");
	// Obtenim identificador per a l'atribut “color” del vertex shader
	colorLoc = glGetAttribLocation (program->programId(), "color");
	// Uniform locations
	transLoc = glGetUniformLocation (program->programId(), "TG");
	projLoc = glGetUniformLocation (program->programId(), "proj");
	viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::calcularParametresEscena(glm::vec3 puntMinim, glm::vec3 puntMaxim) {
    centreEscena.x = (puntMaxim.x + puntMinim.x) / 2.0;
    centreEscena.y = (puntMaxim.y + puntMinim.y) / 2.0;
    centreEscena.z = (puntMaxim.z + puntMinim.z) / 2.0;
    radiEscena = sqrt((puntMinim.x - centreEscena.x)*(puntMinim.x - centreEscena.x)+
                      (puntMinim.y - centreEscena.y)*(puntMinim.y - centreEscena.y)+
                      (puntMinim.z - centreEscena.z)*(puntMinim.z - centreEscena.z));
}

void MyGLWidget::calcularParametresPatricio() {
    glm::vec3 Pminima, Pmaxima;
    Pminima.x = patricio.vertices()[0];
    Pminima.y = patricio.vertices()[1];
    Pminima.z = patricio.vertices()[2];
    
    Pmaxima.x = patricio.vertices()[0];
    Pmaxima.y = patricio.vertices()[1];
    Pmaxima.z = patricio.vertices()[2];
    
    for (unsigned int i = 0; i < patricio.vertices().size(); i += 3) {
        glm::vec3 aux;
        aux.x = patricio.vertices()[i];
        aux.y = patricio.vertices()[i+1];
        aux.z = patricio.vertices()[i+2];
        
        Pminima.x = std::min(Pminima.x, aux.x);
        Pminima.y = std::min(Pminima.y, aux.y);
        Pminima.z = std::min(Pminima.z, aux.z);
        
        Pmaxima.x = std::max(Pmaxima.x, aux.x);
        Pmaxima.y = std::max(Pmaxima.y, aux.y);
        Pmaxima.z = std::max(Pmaxima.z, aux.z);
    }
    alturaPatricio = Pmaxima.y - Pminima.y;

    basePatricio = glm::vec3((Pmaxima.x+Pminima.x)/2.0, Pminima.y, (Pmaxima.z+Pminima.z)/2.0 );
    
}

void MyGLWidget::changeFov(int i) {

    makeCurrent();
	FOV = FOVini -i*0.01;
    alfaIni = FOV/2.0;
	projectTransform();
	update();
}

void MyGLWidget::changeCameraY(int i) {

    makeCurrent();
    angleY = i*0.1;
    viewTransform();
	update();
}

void MyGLWidget::changeCameraX(int i) {

    makeCurrent();
    angleX = i*0.1;
    viewTransform();
	update();
}


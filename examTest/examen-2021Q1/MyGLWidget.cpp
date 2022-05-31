// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  //ExamGLWidget::paintGL();
    // Esborrem el frame-buffer i el depth-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Pintem el terra
    glBindVertexArray (VAO_Terra);
    modelTransformTerra ();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    if (cubosActivados) {
    // Pintem el cub
    glBindVertexArray(VAO_Cub);
    modelTransformCub (2.0, 0.0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    modelTransformCub (2.5, -(M_PI/3));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    modelTransformCub (3, M_PI/3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    else {
    
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
    }
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  //ExamGLWidget::modelTransformCub (1.0, 0.0);
  // En aquest mètode has de substituir aquest codi per construir la 
  // transformació geomètrica (TG) del cub usant els paràmetres adientment
    TG = glm::mat4(1.f);
    TG = glm::rotate(TG, float(rotacion*2*(M_PI/3)), glm::vec3 (0.0, 1.0, 0.0));
    if (angle == 0)TG = glm::translate(TG, glm::vec3 (5, 0.0, 0.0));
    else {
        TG = glm::rotate(TG, angle, glm::vec3 (0.0, 1.0, 0.0));
        TG = glm::translate(TG, glm::vec3 (-5, 0.0, 0.0));
    }
    TG = glm::scale(TG, glm::vec3 (escala/0.5, escala/0.5, escala/0.5));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  //ExamGLWidget::modelTransformPatricio ();
    TG = glm::mat4(1.f);
    TG = glm::rotate(TG, float(rotacion*2*(M_PI/3)), glm::vec3 (0.0, 1.0, 0.0));
    switch (posPatri) {
        case 1:
            TG = glm::translate(TG, glm::vec3(5,0.0,0.0));
            TG = glm::rotate(TG, float(-(M_PI/2.0)), glm::vec3 (0.0, 1.0, 0.0));
            break;
        case 2:
            TG = glm::rotate(TG, float(-(M_PI/3.0)), glm::vec3 (0.0, 1.0, 0.0));
            TG = glm::translate(TG, glm::vec3 (-5, 0.0, 0.0));
            TG = glm::rotate(TG, float((M_PI/2.0)), glm::vec3 (0.0, 1.0, 0.0));
            
            break;
        case 3:
            TG = glm::rotate(TG, float(M_PI/3.0), glm::vec3 (0.0, 1.0, 0.0));
            TG = glm::translate(TG, glm::vec3 (-5, 0.0, 0.0));
            TG = glm::rotate(TG, float((M_PI/2.0)), glm::vec3 (0.0, 1.0, 0.0));
            
            break;
        default:
            break;
    }
    
    TG = glm::scale(TG, glm::vec3 (escala*2, escala*2, escala*2));
    TG = glm::translate(TG, -centreBasePat);
    
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
      View = glm::rotate(View, float(M_PI/2.0), glm::vec3(1, 0, 0));
      View = glm::rotate(View, float(M_PI/2.0), glm::vec3(0, 1, 0));
      View = glm::translate(View, -centreEsc);
    
      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
       
    // Codi per a la projectMatrix de la Càmera-2
      glm::mat4 Proj;  // Matriu de projecció
      Proj = glm::ortho (left, right, bottom, top, zn, zf);
      
      glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      cubosActivados = !cubosActivados;
    break;
	}
  case Qt::Key_1: {
      posPatri = 1;
    break;
	}
  case Qt::Key_2: {
      posPatri = 2;
    break;
	}
  case Qt::Key_3: {
      posPatri = 3;
    break;
	}
  case Qt::Key_F: {
      colFocusGroc = !colFocusGroc;
      if (colFocusGroc){
          colFoc = glm::vec3(1,1,0);
          ExamGLWidget::enviaColFocus();
      }
      else {
          colFoc = glm::vec3(1,1,1);
          ExamGLWidget::enviaColFocus();
      }
          
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      if (camPlanta) emit opticaOrtogonal();
      else emit opticaPrespectiva();
      viewTransform();
      projectTransform();
    break;
	}
  case Qt::Key_Right: {
      ++rotacion;
    break;
	}
  case Qt::Key_Left: {
      --rotacion;
    break;
	}
  case Qt::Key_R: {
      angleY = 0.65;
      angleX = -1.2;
      rotacion = 0;
      posPatri = 1;
      cubosActivados = true;
      colFocusGroc = false;
      if (colFocusGroc){
          colFoc = glm::vec3(1,1,0);
          ExamGLWidget::enviaColFocus();
      }
      else {
          colFoc = glm::vec3(1,1,1);
          ExamGLWidget::enviaColFocus();
      }
      camPlanta = false;
      if (camPlanta) emit opticaOrtogonal();
      else emit opticaPrespectiva();
      viewTransform();
      projectTransform();
      
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::resizeGL (int w, int h) {
    
    #ifdef __APPLE__
      // Aquest codi és necessari únicament per a MACs amb pantalla retina.
      GLint vp[4];
      glGetIntegerv (GL_VIEWPORT, vp);
      ample = vp[2];
      alt = vp[3];
    #else
      ample = w;
      alt = h;
    #endif
    
    float rv = float(ample)/float(alt);
      ra = float(ample)/float(alt);
    if (rv >= 1) {
        left = -radiEsc*rv;
        right = radiEsc*rv;
    }
    else {
        top = radiEsc/rv;
        bottom = -radiEsc/rv;
    }
    
      projectTransform();
}

void MyGLWidget::iniEscena() {
    ExamGLWidget::iniEscena();
    left = -radiEsc;
    right = radiEsc;
    bottom = -radiEsc;
    top = radiEsc;
}

void MyGLWidget::setOpticaPrespectiva() {
    makeCurrent();
    camPlanta = false;
    viewTransform();
    projectTransform();
    update();
}

void MyGLWidget::setOpticaOrtogonal() {
    makeCurrent();
    camPlanta = true;
    viewTransform();
    projectTransform();
    update();
}

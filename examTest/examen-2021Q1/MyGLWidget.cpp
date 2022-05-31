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
      View = glm::rotate(View, float(2*M_PI), glm::vec3(1, 0, 0));
      View = glm::rotate(View, 0.0, glm::vec3(0, 1, 0));
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
      int left = -radiEsc;
      int right = radiEsc;
      int bottom = -radiEsc;
      int top = radiEsc;
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
      viewTransform();
      projectTransform();
    break;
	}
  case Qt::Key_Right: {
      // ...
    break;
	}
  case Qt::Key_Left: {
      // ...
    break;
	}
  case Qt::Key_R: {
      // ...
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


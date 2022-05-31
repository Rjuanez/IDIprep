#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

    public slots:
    void setOpticaPrespectiva();
    void setOpticaOrtogonal();
    
    signals:
    opticaOrtogonal()
    opticaPrespectiva()

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void resizeGL (int width, int height);
    virtual void iniEscena();

  private:
    int printOglError(const char file[], int line, const char func[]);
    
    
    //variables de entorno
    bool cubosActivados = true, colFocusGroc = false;
    unsigned int posPatri = 1;
    int  rotacion = 0;
    
    //variables camara
    float left, right, bottom, top;
};

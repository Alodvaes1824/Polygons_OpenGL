// Este programa demuestra el uso de doble buffer para una animación sin parpadeo.
// Hace girar un cuadrado blanco sobre un fondo negro. Proviene del Capítulo 1
// de la Guía de Programación de OpenGL, pero se han realizado algunos cambios menores
// y la animación se ha implementado correctamente usando temporizadores en lugar de la función idle.
// La animación comienza con el botón izquierdo del mouse y se detiene con el botón derecho.

//#ifdef __APPLE_CC__
//#include <GLUT/glut.h>
//#else
#include <GL/glut.h>
//#endif

// Variable booleana para controlar si la animación está activa.
static bool girando = true;

// Número de fotogramas por segundo que se renderizarán.
static const int FPS = 60;

// Variable global que almacena el ángulo actual de rotación del cuadrado.
// Es mejor mantener el "estado" de la animación de manera global en lugar de
// acumular transformaciones sucesivas. A lo largo del tiempo, la acumulación
// de matrices de transformación genera errores de redondeo.
static GLfloat anguloActualRotacion = 0.0;

// Maneja el evento de cambio de tamaño de la ventana. 
// Primero, asegura que la ventana cubra toda la superficie de dibujo.
// Luego, establece una proyección ortográfica simple con un sistema de coordenadas lógicas 
// que varía de -50 a 50 en la dimensión menor, escalando la dimensión mayor para mantener 
// la isotropía de la ventana.
void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  GLfloat aspect = (GLfloat)w / (GLfloat)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h) {
    // Si el ancho es menor, la vista irá de -50 a 50 en el ancho
    glOrtho(-50.0, 50.0, -50.0/aspect, 50.0/aspect, -1.0, 1.0);
  } else {
    // Si la altura es menor, la vista irá de -50 a 50 en la altura
    glOrtho(-50.0*aspect, 50.0*aspect, -50.0, 50.0, -1.0, 1.0);
  }
}

// Maneja el evento de renderizado de la ventana.
// Primero, limpia la ventana, luego dibuja un cuadrado de 50x50 centrado en el origen 
// y lo rota el número correcto de grados alrededor del eje <0,0,1>.
// La función termina con 'glutSwapBuffers' porque el modo de visualización está configurado 
// con doble buffer. Es necesario llamar a glutSwapBuffers() para mostrar lo que se ha dibujado.
void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(anguloActualRotacion, 0.0, 0.0, 1.0);
  glColor3f(1,0,0);
  glBegin(GL_POLYGON);
  
  glVertex2i(-15.0, -15.0);
  glVertex2i(15.0, -15.0);
  glVertex2i(22.5, 11.25);
  glVertex2i(0.0, 27.5);
  glVertex2i(-22.5, 11.25);
  glEnd();
  glFlush();
  glutSwapBuffers();
}

// Maneja el temporizador aumentando el ángulo de rotación y solicitando 
// que la ventana se vuelva a dibujar, siempre y cuando el programa esté en estado de animación.
// Dado que la función del temporizador solo se ejecuta una vez, es necesario 
// volver a configurarla para que se ejecute continuamente.
void timer(int v) {
  if (girando) {
    anguloActualRotacion += 1.0;
    if (anguloActualRotacion > 360.0) {
      anguloActualRotacion -= 360.0;
    }
    glutPostRedisplay();
  }
  glutTimerFunc(1000/FPS, timer, v);
}

// Maneja los eventos del mouse de la siguiente manera:
// Cuando se presiona el botón izquierdo, la animación comienza.
// Cuando se presiona el botón derecho, la animación se detiene.
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    girando = true;
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    girando = false;
  }
}

// Inicializa GLUT, configura el modo de visualización y la ventana principal, 
// registra las funciones de callback y entra en el bucle principal de eventos.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 500);
  glutCreateWindow("Pentagono Giratorio Odlanyer");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutTimerFunc(100, timer, 0);
  glutMouseFunc(mouse);
  glutMainLoop();
}
//En este programa se presenta un pentágono
//que rota en su eje Z. Para lograr que la animación 
//no tenga parpadeos ni distorsiones innesperadas
//se utilizan 2 buffer
#include <GL/glut.h>

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

void display() {
//Esta función limpia la pantalla del color del buffer
  glClear(GL_COLOR_BUFFER_BIT);
//Inicializa la matriz 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
//La sintaxis de esta función nos dice que glrotatef
//es tipo float, además nos pide 4 valores; el ángulo, 
//el eje X, Y, Z. 
  glRotatef(anguloActualRotacion, 0.0, 0.0, 1.0);
//se le asigna un color en la gama rojo, verde, azul. 
  glColor3f(1,0,0);
//Declaramos el tipo de dibujado que queremos. 
  glBegin(GL_POLYGON); 
//Hacemos los vértices de la figura. 
  glVertex2i(-15.0, -15.0);
  glVertex2i(15.0, -15.0);
  glVertex2i(22.5, 11.25);
  glVertex2i(0.0, 27.5);
  glVertex2i(-22.5, 11.25);
//cerramos el glbegin
  glEnd();
//pintamos los vertices
  glFlush();
//cambiamos de buffer o pantalla
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
// indica posición donde se desplegará la ventana
  glutInitWindowPosition(80, 80);
// indica el tamaño o la resolución que tomará la ventana
  glutInitWindowSize(800, 500);
// nombre que mostrará el marco de la ventana
  glutCreateWindow("Pentagono Giratorio Odlanyer");
// las siguientes son llamadas a funciones anteriores
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutTimerFunc(100, timer, 0);
  glutMouseFunc(mouse);
// indica a la función Main ser un Loop
  glutMainLoop();
}

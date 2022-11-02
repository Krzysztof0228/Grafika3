#include <iostream>
#include <windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#include <cmath>

#define GLEW_STATIC
#define FREEGLUT_STATIC

#define M_PI 3.14159265358979323846

using namespace std;

typedef float point3[3];
static GLfloat theta[] = { 0.0,0.0,0.0 };

int model = 1; // 1-punkty, 2-siatka, 3-wypełnione trójkąty

// Funkcja rysująca osie układu współrzędnych
void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// początek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// początek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  początek i koniec obrazu osi z

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
		glVertex3fv(x_min);
		glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
		glVertex3fv(y_min);
		glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
		glVertex3fv(z_min);
		glVertex3fv(z_max);
	glEnd();
}

// Wzory opisujące jajko
float policzX(float u, float v)
{
	float PIV = M_PI * v;
	return((-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(PIV));
}

float policzY(float u, float v)
{
	return(160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2));
}

float policzZ(float u, float v)
{
	float PIV = M_PI * v;
	return((-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(PIV));
}

struct Punkt
{
	float x;
	float y;
	float z;
	float xRGB;
	float yRGB;
	float zRGB;
};

const int N = 20;

struct Punkt Jajko[N][N];

float krok = 1.0 / N;

// Funkcja wypełniająca tablicę punktami jajka
void policz()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Jajko[i][j].x = policzX(i * krok, j * krok);
			Jajko[i][j].y = policzY(i * krok, j * krok);
			Jajko[i][j].z = policzZ(i * krok, j * krok);

			Jajko[i][j].xRGB = ((double)rand() / (RAND_MAX));
			Jajko[i][j].yRGB = ((double)rand() / (RAND_MAX));
			Jajko[i][j].zRGB = ((double)rand() / (RAND_MAX));
		}
	}
}

void Jajo()
{
	policz();

	// Model jajka narysowany za pomocą punktów
	if (model == 1)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				glBegin(GL_POINTS);
				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(Jajko[i][j].x, Jajko[i][j].y - 5, Jajko[i][j].z);
				glEnd();
			}
		}
	}

	// Model jajka narysowany za pomocą siatki
	else if (model == 2)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				//linie pionowe
				if (i + 1 < N)
				{
					glBegin(GL_LINES);
						glVertex3f(Jajko[i][j].x, Jajko[i][j].y - 5, Jajko[i][j].z);
						glVertex3f(Jajko[i + 1][j].x, Jajko[i + 1][j].y - 5, Jajko[i + 1][j].z);
					glEnd();
				}
				//dorysowanie linii pionowych na dole jajka
				else
				{
					glBegin(GL_LINES);
						glVertex3f(Jajko[i][j].x, Jajko[i][j].y - 5, Jajko[i][j].z);
						glVertex3f(Jajko[0][0].x, Jajko[0][0].y - 5, Jajko[0][0].z);
					glEnd();
				}

				//linie poziome
				if (j + 1 < N)
				{
					glBegin(GL_LINES);
						glVertex3f(Jajko[i][j].x, Jajko[i][j].y - 5, Jajko[i][j].z);
						glVertex3f(Jajko[i][j + 1].x, Jajko[i][j + 1].y - 5, Jajko[i][j + 1].z);
					glEnd();
				}
				//linie poziome łączące połowy jajka
				else
				{
					glBegin(GL_LINES);
						glVertex3f(Jajko[i][j].x, Jajko[i][j].y - 5, Jajko[i][j].z);
						glVertex3f(-Jajko[i][0].x, Jajko[i][j].y - 5, 0);
					glEnd();
				}

				//linie ukośne dla jednej połowy jajka
				if (j + 1 < N && i != 0 && i<(int)N/2-1)
				{
					glBegin(GL_LINES);
						glVertex3f(Jajko[i][j].x, Jajko[i][j].y - 5, Jajko[i][j].z);
						glVertex3f(Jajko[i + 1][j + 1].x, Jajko[i + 1][j + 1].y - 5, Jajko[i + 1][j + 1].z);
					glEnd();
				}
				//linie ukośne dla drugiej połowy jajka
				if (j + 1 < N && i > 1 && i>=(int)N/2+1)
				{
					glBegin(GL_LINES);
						glVertex3f(Jajko[i][j].x, Jajko[i][j].y - 5, Jajko[i][j].z);
						glVertex3f(Jajko[i - 1][j + 1].x, Jajko[i - 1][j + 1].y - 5, Jajko[i - 1][j + 1].z);
					glEnd();
				}
				// linie ukośne łączące połowy jajka
				if (i != 0 && j == N - 2 && i < (int)N / 2 - 1)
				{
					glBegin(GL_LINES);
						glVertex3f(Jajko[i][N - 1].x, Jajko[i][N - 1].y - 5, Jajko[i][N - 1].z);
						glVertex3f(-Jajko[i+1][0].x, Jajko[i+1][j].y - 5, -Jajko[i+1][0].z);
					glEnd();
				}

				if (i < (int)N / 2 - 1 && i != 0 && j == N - 2)
				{
					glBegin(GL_LINES);
						glVertex3f(-Jajko[i][N - 1].x, Jajko[i][N - 1].y-5, -Jajko[i][N - 1].z);
						glVertex3f(Jajko[i+1][0].x, Jajko[i+1][0].y-5, -Jajko[i+1][0].z);
					glEnd();
				}
			}
		}
	}

	// Model jajka narysowany za pomocą trójkątów
	else if (model == 3)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (j < N - 1)
				{
					//Trójkąty połowy góry oraz dołu i trójkąty skośne w jedną stronę
					glBegin(GL_TRIANGLES);
						glColor3f(Jajko[i][j].xRGB, Jajko[i][j].yRGB, Jajko[i][j].zRGB);
						glVertex3f(Jajko[i][j].x, Jajko[i][j].y-5, Jajko[i][j].z);
						glColor3f(Jajko[(i+1)%N][j].xRGB, Jajko[(i+1)%N][j].yRGB, Jajko[(i+1)%N][j].zRGB);
						glVertex3f(Jajko[(i + 1) % N][j].x, Jajko[(i + 1) % N][j].y-5, Jajko[(i + 1) % N][j].z);
						glColor3f(Jajko[i][j+1].xRGB, Jajko[i][j+1].yRGB, Jajko[i][j+1].zRGB);
						glVertex3f(Jajko[i][j+1].x, Jajko[i][j+1].y-5, Jajko[i][j+1].z);
					glEnd();
					//Rysowanie drugiej połowy dołu i góry oraz trójkąty skośne w drugą stronę
					glBegin(GL_TRIANGLES);
						glColor3f(Jajko[(i + 1) % N][j].xRGB, Jajko[(i + 1) % N][j].yRGB, Jajko[(i + 1) % N][j].zRGB);
						glVertex3f(Jajko[(i + 1) % N][j].x, Jajko[(i + 1) % N][j].y-5, Jajko[(i + 1) % N][j].z);
						glColor3f(Jajko[(i + 1) % N][j+1].xRGB, Jajko[(i + 1) % N][j+1].yRGB, Jajko[(i + 1) % N][j+1].zRGB);
						glVertex3f(Jajko[(i + 1) % N][j+1].x, Jajko[(i + 1) % N][j+1].y-5, Jajko[(i + 1) % N][j+1].z);
						glColor3f(Jajko[i][j + 1].xRGB, Jajko[i][j + 1].yRGB, Jajko[i][j + 1].zRGB);
						glVertex3f(Jajko[i][j + 1].x, Jajko[i][j + 1].y-5, Jajko[i][j + 1].z);
					glEnd();
				}
				//Trójkąty łączące połowy jajka
				else
				{
					glBegin(GL_TRIANGLES);
						glColor3f(Jajko[(i + 1) % N][j].xRGB, Jajko[(i + 1) % N][j].yRGB, Jajko[(i + 1) % N][j].zRGB);
						glVertex3f(Jajko[(i + 1) % N][j].x, Jajko[(i + 1) % N][j].y-5, Jajko[(i + 1) % N][j].z);
						glColor3f(Jajko[(N - i) % N][0].xRGB, Jajko[(N - i) % N][0].yRGB, Jajko[(N - i) % N][0].zRGB);
						glVertex3f(Jajko[(N - i) % N][0].x, Jajko[(N - i) % N][0].y-5, Jajko[(N - i) % N][0].z);
						glColor3f(Jajko[N - i - 1][0].xRGB, Jajko[N - i - 1][0].yRGB, Jajko[N - i - 1][0].zRGB);
						glVertex3f(Jajko[N - i - 1][0].x, Jajko[N - i - 1][0].y-5, Jajko[N - i - 1][0].z);
					glEnd();
				}
				if (i > 0)
				{
					glBegin(GL_TRIANGLES);
						glColor3f(Jajko[i][j].xRGB, Jajko[i][j].yRGB, Jajko[i][j].zRGB);
						glVertex3f(Jajko[i][j].x, Jajko[i][j].y-5, Jajko[i][j].z);
						glColor3f(Jajko[(i+1) % N][j].xRGB, Jajko[(i+1) % N][j].yRGB, Jajko[(i+1) % N][j].zRGB);
						glVertex3f(Jajko[(i+1) % N][j].x, Jajko[(i+1) % N][j].y-5, Jajko[(i+1) % N][j].z);
						glColor3f(Jajko[N - i][0].xRGB, Jajko[N - i][0].yRGB, Jajko[N - i][0].zRGB);
						glVertex3f(Jajko[N - i][0].x, Jajko[N - i][0].y-5, Jajko[N - i][0].z);
					glEnd();
				}
			}
		}
	}
	else if (model == 4) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glutWireTeapot(3.0);
	}
}

// Funkcja obracająca obiekt
void spinEgg()
{
	theta[0] -= 0.02;
	if (theta[0] > 360.0)
	{
		theta[0] -= 360.0;
	}

	theta[1] -= 0.02;
	if (theta[1] > 360.0)
	{
		theta[1] -= 360.0;
	}

	theta[2] -= 0.02;
	if (theta[2] > 360.0)
	{
		theta[2] -= 360.0;
	}

	glutPostRedisplay();
}

// Funkcja odczytująca klawisze
void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1; // Punkty
	if (key == 'w') model = 2; // Linie
	if (key == 's') model = 3; // Trójkąty
	if (key == 'c') model = 4; // Czajnik
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczącym

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wyżej
	
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);
	Jajo();
	
	glFlush();
	// Przekazanie poleceń rysujących do wykonania

	glutSwapBuffers();
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcący (wypełnienia okna) ustawiono na czarny
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  określającej proporcję
	// wymiarów okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkościokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Przełączenie macierzy bieżącej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bieżącej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie współczynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest określenie tak zwanej
	// przestrzeni ograniczającej pozwalającej zachować właściwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczjącej służy funkcja
	// glOrtho(...)            
	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else
		glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	// Przełączenie macierzy bieżącej na macierz widoku modelu                                   

	glLoadIdentity();
	// Czyszcenie macierzy bieżącej
}

int main()
{
	cout << "p - punkty"<<endl;
	cout << "w - siatka" << endl;
	cout << "s - trojkaty" << endl;
	cout << "c - czajnik" << endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Jajko 3-D");

	glutDisplayFunc(RenderScene);
	
	glutIdleFunc(spinEgg);

	glutReshapeFunc(ChangeSize);
	     
	MyInit();
	
	glEnable(GL_DEPTH_TEST);
	
	glutKeyboardFunc(keys);

	glutMainLoop();
	
	return 0;
}


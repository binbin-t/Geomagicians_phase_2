
#include "basicsP2\pointSetArray.h"
#include "basicsP2\trist.h"

#include "math.h"
#include <iostream>
#include <fstream>
#include <gl\glut.h>
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>

using namespace std;

Trist triangles;
DWORD dy_ms;
// These three functions are for those who are not familiar with OpenGL, you can change these or even completely ignore them

void drawAPoint(double x,double y)
{
		glPointSize(5);
		glBegin(GL_POINTS);
		glColor3f(0,0,0);
		glVertex2d(x,y);
		glEnd();
		glPointSize(1);
}

void drawALine(double x1,double y1, double x2, double y2)
{
		glPointSize(1);
		glBegin(GL_LINE_LOOP);
		glColor3f(0,0,1);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
		glEnd();
		glPointSize(1);
}

void drawATriangle(double x1,double y1, double x2, double y2, double x3, double y3)
{
		glBegin(GL_POLYGON);
		glColor3f(0,0.5,0);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
		glVertex2d(x3,y3);
		glEnd();

}



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();


	// draw your output here (erase the following 3 lines)
	drawAPoint(100,100);
	drawALine(200,200,300,300);
	drawATriangle(400,400,400,500,500,500);

	glPopMatrix();
	glutSwapBuffers ();
}

void reshape (int w, int h)
{

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
}



void readFile(){

	string line_noStr;

	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation
	string outputAns = "Answer of your computation"; // the answer you computed

	ifstream inputFile("input.txt",ios::in);


	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << "input.txt" << "\"";
		return;
	}

	while(inputFile.good()){

		getline(inputFile,line);
		if(line.empty()) {
			command = "";
			continue; 
		}// in case the line has nothing in it

		Sleep(dy_ms);

		stringstream linestream(line);

		linestream >> line_noStr;
		linestream >> command;         // get the command

		if(!command.compare("AP")){
			linestream >> numberStr;
			LongInt p1 = LongInt::LongInt(numberStr.c_str());
			
			linestream >> numberStr;
			LongInt p2 = LongInt::LongInt(numberStr.c_str());
			
			triangles.addPoint(p1, p2);
		
		}
		else if(!command.compare("OT")){
			linestream >> numberStr;
			int p1Idx = atoi(numberStr.c_str());
			linestream >> numberStr;			
			int p2Idx = atoi(numberStr.c_str());
			linestream >> numberStr;
			int p3Idx = atoi(numberStr.c_str());

			triangles.makeTri(p1Idx,p2Idx,p3Idx, true);
			
		}
		else if(!command.compare("IP")){
			linestream >> numberStr;
			LongInt px = LongInt::LongInt(numberStr.c_str());
			linestream >> numberStr;
			LongInt py = LongInt::LongInt(numberStr.c_str());

			int pIdx = triangles.addPoint(px,py), p1Idx, p2Idx, p3Idx;
			OrTri tri = triangles.inTriangle(pIdx);
			if(tri >= 0){
				triangles.getVertexIdx(tri, pIdx, p2Idx, p3Idx);
				triangles.delTri(tri);

				triangles.makeTri(pIdx, p1Idx, p2Idx, true);
				triangles.makeTri(pIdx, p2Idx, p3Idx, true);
				triangles.makeTri(pIdx, p3Idx, p1Idx, true);
			}
		}
		else if(!command.compare("DY")){
			linestream >> numberStr;
			dy_ms = atol(numberStr.c_str());
			if(dy_ms<0){
				dy_ms = 0;
			}
		}
		else{
			cerr << "Exception: Wrong input command" << endl;
		}
	}

}

void writeFile()
{
	ofstream outputFile("output.txt",ios::out, ios_base::trunc);
	int no_line = 1;
	int nbPoint = triangles.noPt();
	int p1Idx, p2Idx, p3Idx;
	OrTri tri;
	LongInt px, py;
	for(int i=0; i<nbPoint-1; i++){
		triangles.getPoint(i,px,py);
		outputFile << no_line << ": AP " << px.printOut().c_str() << " " << py.printOut().c_str() << endl;
		no_line++;
	}
	
	for(int i=0; i<triangles.noTri(); i++){
		tri = i << 3;
		triangles.getVertexIdx(tri, p1Idx, p2Idx, p3Idx);
		outputFile << no_line << ": OT " << p1Idx << " " << p2Idx << " " << p3Idx << endl;
		no_line++;
	}
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'r':
		case 'R':
			readFile();
		break;

		case 'w':
		case 'W':
			writeFile();
		break;

		case 'Q':
		case 'q':
			exit(0);
		break;

		default:
		break;
	}

	glutPostRedisplay();
}



void mouse(int button, int state, int x, int y)
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_RIGHT_BUTTON)&&(state == GLUT_UP))
	{

	}

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	cout<<"CS5237 Phase II"<< endl<< endl;
	cout << "Right mouse click: OT operation"<<endl;
	cout << "Q: Quit" <<endl;
	cout << "R: Read in control points from \"input.txt\"" <<endl;
	cout << "W: Write control points to \"input.txt\"" <<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1000, 700);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS5237 Phase II");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}

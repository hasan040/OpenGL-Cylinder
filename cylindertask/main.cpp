
#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>




#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};


double _z_angle = 0;
double _x_angle = 0;
double _x_cdr_angle = 0;

double _own_angle = 0;

double rdn_angle = (pi * 3)/180;

double _distance = 3;

double ax_length = 800;

int _bullet = 0;

int total_bullet = 0;

double z_ang_arr[1000];
double x_ang_arr[1000];
double x_cdr_ang_arr[1000];

point camPos;

point u_vec;
point r_vec;
point l_vec;



void drawAxes()
{
	if(drawaxes==1)
	{
	    glColor3f(1, 1, 1);
		glBegin(GL_LINES);{
		    //glColor3f(1, 1, 1);
			glVertex3f( ax_length,0,0);
			glVertex3f(-ax_length,0,0);



			//glColor3f(1, 0, 0);
			glVertex3f(0,0, ax_length);
			glVertex3f(0,0,-ax_length);

            //glColor3f(0,1,0);
			glVertex3f(0,-ax_length,0);
			glVertex3f(0, ax_length,0);
		}
		glEnd();


	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(.6, .6, .6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a,double val,double val2,double val3)
{
    glColor3f(val,val2,val3);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        if(i & 1) glColor3f(1,1,1);
        else glColor3f(0,0,0);

        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawCylinder(double radius,int segments,double length)
{

    int i;
    struct point points[100];

    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }

    double _dist = 0;

    for(int k=0;k<length*10;k++){

        glPushMatrix();
        glTranslatef(0,_dist,0);
        glRotatef(90,1,0,0);

        for(i=0;i<segments;i++)
        {
            if(i & 1) glColor3f(0,0,0);
            else glColor3f(1,1,1);

            glBegin(GL_LINES);
            {
                glVertex3f(points[i].x,points[i].y,0);

                glVertex3f(points[i+1].x,points[i+1].y,0);

            }
            glEnd();
        }

        glPopMatrix();
         _dist += 0.08;

    }





}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);


		for(j=0;j<slices;j++)
		{
		    if(j & 1)
                glColor3f(0,0,0);
            else
                glColor3f(1,1,1);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawMyCylinder(double radius,int slices,int stacks)
{
	struct point points[200][200];
	int i,j;
	double h;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		//r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=radius*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=radius*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}

	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);


		for(j=0;j<slices;j++)
		{
		    if(j & 1)
                glColor3f(0,0,0);
            else
                glColor3f(1,1,1);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawcdTail(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}

	for(i=0;i<stacks/2;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);


		for(j=0;j<slices;j++)
		{
		    if(j & 1)
                glColor3f(0,0,0);
            else
                glColor3f(1,1,1);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

}


void drawTail(double radius)
{
    double _dist = 0;

    double _segment = radius ;

    for(int j=0;j<2000;j++){
        glPushMatrix();
        {
            glRotatef(90,1,0,0);
            glTranslatef(0,_dist,0);

            drawCircle(radius,_segment);
        }
        glPopMatrix();
        radius += .009;
        _dist += .00099;

    }


}

void drawSS()
{
    glPushMatrix();
    {
        glRotatef(_z_angle,0,0,1);


        //.......outer.........

        glPushMatrix();
        {
            glRotatef(90,1,0,0);
            drawSphere(50,51,50); //1st half sphere
        }
        glPopMatrix();

        //......second outer.............

        glPushMatrix();
        {
//...............%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
           glRotatef(_x_angle,1,0,0);

           glPushMatrix();
            {
                glRotatef(-90,1,0,0);
                drawSphere(50,51,50);  //second half sphere
            }
            glPopMatrix();


            //.......only cylinder...............

            glPushMatrix();
            {
                //.........$$$$$$$$$$$$$$$$$$............

                glRotatef(_x_cdr_angle,1,0,0);

                glRotatef(_own_angle,0,1,0);

                 glPushMatrix();
                {

                    glTranslatef(0,70,0);
                    glRotatef(90,1,0,0);
                    drawSphere(20,30,30);  //smaller sphere
                }
                glPopMatrix();

                glPushMatrix();
                {
                    //glTranslatef(0,70,0);
                    glPushMatrix();
                    glTranslatef(0,90,0);
                    glRotatef(90,1,0,0);
                    //drawCylinder(20,30,110);  //cylinder
                    drawMyCylinder(20,30,30);
                    glPopMatrix();
                    glPushMatrix();
                    glTranslatef(0,130,0);
                    glRotatef(90,1,0,0);
                    drawMyCylinder(20,30,30);
                    glPopMatrix();
                    glPushMatrix();
                    glTranslatef(0,170,0);
                    glRotatef(90,1,0,0);
                    drawMyCylinder(20,30,30);
                    glPopMatrix();



                }
                glPopMatrix();

                glPushMatrix();
                {
                    glTranslatef(0,207,0); //tail
                    glRotatef(90,1,0,0);
                    //drawTail(20);
                    drawcdTail(28,30,30);

                }
                glPopMatrix();


                //.........$$$$$$$$$$$$$$$$$$............
            }
            glPopMatrix();

            //.......cylinder only..............



//...............%%%%%%%%%%%%%%%%%%%%%%%%%


        }
        glPopMatrix();

         //.......second outer..............



        //......outer.........

    }
    glPopMatrix();







    glPushMatrix();
    {
         glRotatef(90,1,0,0);
         glTranslatef(0,0,-400);
         drawSquare(150,.6,.6,.6);
    }
    glPopMatrix();


    if(total_bullet > 0)
    {
        for(int j=0;j<total_bullet;j++){
            //..............

            double ang_1 = x_ang_arr[j];
            double ang_2 = x_cdr_ang_arr[j];

            if(ang_1 <= 15 && ang_1 >= -15 && ang_2 <= 15 && ang_2 >= -15){
                glPushMatrix();
                {

                    glRotatef(z_ang_arr[j],0,0,1);
                    glRotatef(x_ang_arr[j],1,0,0);
                    glRotatef(x_cdr_ang_arr[j],1,0,0);

                    glPushMatrix();
                    {
                        glRotatef(90,1,0,0);
                        glTranslatef(0,0,-390);
                        drawSquare(5,1,0,0);
                    }
                    glPopMatrix();
                }
                glPopMatrix();
            }

            //..............
        }
    }

    //drawMyCylinder(30,60,30);




}




void op_rotate(point & fx,point & rt1,point & rt2,double _rdn)
{
    //double _rdn = (pi * dg_angle)/180;

    point cross1,cross2;

    cross1.x = fx.y * rt1.z - fx.z * rt1.y;
    cross1.y = fx.z * rt1.x - fx.x * rt1.z;
    cross1.z = fx.x * rt1.y - fx.y * rt1.x;

    cross2.x = fx.y * rt2.z - fx.z * rt2.y;
    cross2.y = fx.z * rt2.x - fx.x * rt2.z;
    cross2.z = fx.x * rt2.y - fx.y * rt2.x;

    rt1.x = rt1.x * cos(_rdn) + cross1.x * sin(_rdn);
    rt1.y = rt1.y * cos(_rdn) + cross1.y * sin(_rdn);
    rt1.z = rt1.z * cos(_rdn) + cross1.z * sin(_rdn);

    rt2.x = rt2.x * cos(_rdn) + cross2.x * sin(_rdn);
    rt2.y = rt2.y * cos(_rdn) + cross2.y * sin(_rdn);
    rt2.z = rt2.z * cos(_rdn) + cross2.z * sin(_rdn);







}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case 'q':
		    if(_z_angle < 30){
                _z_angle ++;
		    }
            break;

        case 'w':
            if(_z_angle > -30){
                _z_angle--;
            }
            break;

        case 'e':
            _x_angle++;
            break;

        case 'r':
            _x_angle--;
            break;
        case 'a':
            _x_cdr_angle++;
            break;

        case 's':
            _x_cdr_angle--;
            break;

        case 'd':
            _own_angle--;
            break;
        case 'f':
            _own_angle++;
            break;

        //for rotation
        case '2':
            op_rotate(u_vec,l_vec,r_vec,-rdn_angle);
            break;

        case '1':
            op_rotate(u_vec,l_vec,r_vec,rdn_angle);
            break;

        case '3':
            op_rotate(r_vec,u_vec,l_vec,rdn_angle);
            break;
        case '4':
            op_rotate(r_vec,u_vec,l_vec,-rdn_angle);
            break;
        case '5':
            op_rotate(l_vec,u_vec,r_vec,rdn_angle);
            break;
        case '6':
            op_rotate(l_vec,u_vec,r_vec,-rdn_angle);
            break;


		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			camPos.x -= _distance*l_vec.x;
			camPos.y -= _distance*l_vec.y;
			camPos.z -= _distance*l_vec.z;


			break;
		case GLUT_KEY_UP:		// up arrow key
			camPos.x += _distance*l_vec.x;
			camPos.y += _distance*l_vec.y;
			camPos.z += _distance*l_vec.z;

			break;

		case GLUT_KEY_RIGHT:
			camPos.x += _distance*r_vec.x;
			camPos.y += _distance*r_vec.y;
			camPos.z += _distance*r_vec.z;

			break;
		case GLUT_KEY_LEFT:
			camPos.x -= _distance*r_vec.x;
			camPos.y -= _distance*r_vec.y;
			camPos.z -= _distance*r_vec.z;

			break;

		case GLUT_KEY_PAGE_UP:
			camPos.x += _distance*u_vec.x;
			camPos.y += _distance*u_vec.y;
			camPos.z += _distance*u_vec.z;

			break;
		case GLUT_KEY_PAGE_DOWN:
			camPos.x -= _distance*u_vec.x;
			camPos.y -= _distance*u_vec.y;
			camPos.z -= _distance*u_vec.z;

			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
		    z_ang_arr[total_bullet] = _z_angle;
		    x_ang_arr[total_bullet] = _x_angle;
		    x_cdr_ang_arr[total_bullet] = _x_cdr_angle;
            total_bullet++;

            //_bullet = 1;

			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	1,0,0);




	gluLookAt(camPos.x,camPos.y,camPos.z,
           camPos.x + l_vec.x,camPos.y + l_vec.y,camPos.z + l_vec.z,
           u_vec.x,u_vec.y,u_vec.z);






	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//added components for camera position

	camPos.x = 150;
	camPos.y = 150;
	camPos.z = 0;

	u_vec.x = 0;
	u_vec.y = 0;
	u_vec.z = 1;

	r_vec.x = -1/sqrt(2);
	r_vec.y = 1/sqrt(2);
	r_vec.z = 0;

	l_vec.x = -1/sqrt(2);
	l_vec.y = -1/sqrt(2);
	l_vec.z = 0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(100,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}



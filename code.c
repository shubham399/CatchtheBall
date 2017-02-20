/* Simple Game */
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<math.h>
#include<stdlib.h>
#include<GL/glut.h>



/*Variables Declaration Begin */
#define PI 3.14159265f
#define TIME_MAX 60
#define BALL_MAX 10
int onclickred=0.0; //to change color if a button is clicked
struct Ball
{
GLfloat Red,Green,Blue;//Colour of the Ball
GLfloat X,Y;//Center Cordinates
GLfloat Radius;//Radius of the Ball
GLfloat XMin,XMax,YMax,YMin;//Maximum Movent Cordinates
GLfloat XSpeed,YSpeed;//Movement Speed
};
/*Declaring the Balls */
struct Ball A[BALL_MAX];
struct Ball B;
/*Other Variables */
GLfloat wmax=1300,hmax=740;//Screen resolution (width and height of the screen)
int score=0;
float time=0;
int level=1,upflag=1;
int screen=1,xflag[BALL_MAX],yflag[BALL_MAX],bm=BALL_MAX;//Screen Flags for Move A
float factor=2.02;//Speed factor
int refreshMillis=30;//Screen Referesh Rate
int bl=0;//blink effect variable
float rx1=-0.5,ry1=-0.6,rx2=0.4,ry2=0.5;//Red Zone cordinates
GLdouble clipAreaXLeft,clipAreaXRight,clipAreaYBottom,clipAreaYTop;//Cliping Area
/* Variables Declaration Over*/



/* Data Generation and Control Functions Start */
//Check For Ball to Ball Collsion
int collsion(struct Ball P,struct Ball Q,GLfloat off)
{
GLfloat disX=P.X-Q.X;
GLfloat disY=P.Y-Q.Y;
GLfloat dist=sqrt((disX*disX)+(disY*disY));
return dist <=(P.Radius+Q.Radius-off);
}
// Genrate A Random Postion on Screen
float randpos(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
// Get XY value
void getXY(int n)
{
	int l;
	A[n].Radius=randpos(0.05,0.1);
	A[n].X=randpos(-1.33,1.33);
	A[n].Y=randpos(-1.0,1.0);
	for(l=0;l<=n;l++)
	{
		if(l!=n)
		{
		int c=collsion(A[l],A[n],0);
		//printf("Collison=%d\n",c);
		if(c )
		{
		//printf("Collison%d\n",n);
		A[n].Radius=randpos(0.05,0.1);
		A[n].X=randpos(-1.33,1.33);
		A[n].Y=randpos(-1.0,1.0);
		--l;//To Recheck
		}
		}
	}
}
// To Check Weather the Ball is in Red Zone
int inredzone(struct Ball P)
{
 if(P.X>(rx1-P.Radius)&&P.X<(rx2+P.Radius))
  if(P.Y>(ry1-P.Radius)&&P.Y<(ry2+P.Radius))
   return 1;
 return 0;
}
/* Data Generation and Control Functions  End*/



/*Initialize Functions Begin */
//To Reset GLSettings
void initGL()
{
 int i1;
 if(!upflag)
 {
  rx1-=0.1;
  ry1-=0.05;
  rx2+=0.1;
  ry2+=0.05;
 }/*Initalize B */
 B.Red=1.0f;
 B.Green=1.0f;
 B.Blue=1.0f;
 B.Radius=0.05f;
 B.X=-1.33+B.Radius;
 B.Y=-1.0+B.Radius;
 B.XSpeed=0.007f;
 B.YSpeed=0.007f;
 if(upflag)
 {
  B.XMin=0;
  B.XMax=0;
  B.YMax=0;
  B.YMin=0;
 }
 for(i1=0;i1<bm;i1++)
 {
  xflag[i1]=0;
  yflag[i1]=0;
  /*Initalize A*/
  A[i1].Blue=randpos(0,1);
  A[i1].Red=randpos(0,0.9);
  A[i1].Green=randpos(0,1);
  getXY(i1);
  if(upflag)
  {
   A[i1].XSpeed=0.005f;
   A[i1].YSpeed=0.005f;
   A[i1].XMin=0;
   A[i1].XMax=0;
   A[i1].YMax=0;
   A[i1].YMin=0;
  }
 }
 upflag=0;
 /*Clear Screen*/
 glClearColor(1.0,1.0,1.0,1.0);
}
//Reset the Game
void resetgame()
{
    int i;
    for(i=0;i<bm;i++)
    {
        A[i].XSpeed=0.005f;
        A[i].YSpeed=0.005f;
    }
    rx1=-0.5;
    ry1=-0.5;
    rx2=0.5;
    ry2=0.5;
    score=0;
    time=0.0;
    level=1;
    initGL();
    bm=BALL_MAX;
}
/* Initalize Functions End */



/* BackGround Related Functions Begin */
void drawbackgroundball(int s)//Draw the Background Balls
{
   	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.01,0.01,s/6.0);
   	if (s==0)
	glColor3f(1.0,1.0,1.0);
	int num=100,i;
	float r=0.9*s+0.5;
	GLfloat angle;
	glVertex2f(1000.0f,0.0f);
	for(i=0;i<=num;i++)
	{
		angle=i*2.0f*PI/num;
		glVertex2f(cos(angle)*r,sin(angle)*r);
	}
	glEnd();
}//Function Ends
void drawbackground()//Draw the Background
{
    /**/
    glTranslatef(-0.55,0.-0.08,0.0);
    drawbackgroundball(3);
    drawbackgroundball(2);
    drawbackgroundball(1);
    drawbackgroundball(0);
    glTranslatef(0.55,0.08,0.0);
}
/* BackGround Related Functions  Ends*/



/*String Displaying Functions Begin */
//Required To Display Strings Declaration Needed
void drawbutton(GLint x1,GLint y1,GLint x2,GLint y2);
void distring(const GLfloat sx, const GLfloat sy, const char* s)//Display a String
{
int l=0,i;
l=strlen(s);
glRasterPos2f(sx,sy);
for(i=0;i<l;i++)
glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,s[i]);
}// Function END
void displayLevel()//Display Level 
{

	glLoadIdentity();
	glColor3f(1.0,1.0,0.0);
	glTranslatef(clipAreaXLeft+0.1f,clipAreaYTop-0.2f,0.0f);
	distring(0.0,0.0,"Level: ");
	char levels [50];
	sprintf(levels, "%d", level);
	glColor3f(1.0,0.0,1.0);
	distring(0.3,0.0,levels);
}
void gameover()//Game Over Screen Text 
{
    glLoadIdentity();
    drawbackground();
    glClearColor(1,1,1,1);
	glTranslatef(0.0f,0.0f,0.0f);
	glColor3f(0.1,0.1,0.8);
	distring(-0.4,0.12,"G A M E   O V E R");
	distring(-0.4,0.11,"_______________");
	distring(-0.4,0.1," ______________");
	glColor3f(0.10,0.10,1.0);
	distring(-0.5,-0.05,"Total Score: ");
	char levels [50];
	sprintf(levels, "%d", score);
	distring(0.1,-0.05,levels);
    bl++;
    bl=bl%20;
    if(bl<10)// blinking effect
        glColor3f(1.0,1.0,1.0);
    else
        glColor3f(0.0,0.0,0.0);
	distring(-0.5,-0.2,"Press n or N to Play Again Now!!!");
	distring(-0.5,-0.3,"Press x or X to Quit Now");
    drawbutton(330,hmax-1050,-700,hmax-1150);
	glColor3f(0.0,0.0,1.0);
    distring(-0.5,-0.5,"BACK TO MAIN MENU");
}
//Display the Score 
void displayScore()
{
	glLoadIdentity();
	glColor3f(1.0,1.0,0.0);
	glTranslatef(clipAreaXLeft+0.1f,clipAreaYTop-0.3f,0.0f);
	distring(0.0,0.0,"Total Score: ");
	char levels [50];
	sprintf(levels, "%d", score);
	glColor3f(1.0,0.0,1.0);
	distring(0.5,0.0,levels);
}
//Display Time Left
void displaytimeleft()
{
	glLoadIdentity();
	glColor3f(1.0,1.0,0.0);
	glTranslatef(clipAreaXLeft+0.1f,clipAreaYTop-0.4f,0.0f);
	distring(0.0,0.0,"Time Left: ");
	char levels [50];
	sprintf(levels, "%d Seconds", (TIME_MAX-(int)time));
	glColor3f(1.0,0.0,1.0);
	distring(0.5,0.0,levels);
}
/*Display Welcome Message*/
void displaywelcome()
{ /********MAIN MENU************/
	drawbackground();
	glLoadIdentity();
	glTranslatef((clipAreaXLeft+clipAreaXRight)/2,(clipAreaYTop+clipAreaYBottom)/2,0.0f);
    glColor3f(0.90,0.90,0.990);
	distring(-0.35,+0.6," T H E   C R A Z Y   B A L L S ");
    glColor3f(0.90,0.90,0.990);
	distring(-0.34,+0.57," ______________________");
	distring(-0.34,+0.55,"  _____________________");
    drawbutton(129,hmax-525,-279,hmax-625);
	glColor3f(0.0,0.0,0.99);
	distring(-0.17,0.2,"    PLAY");
    drawbutton(129,hmax-675,-279,hmax-775);
	glColor3f(0.0,0.0,0.99);
	distring(-0.16,0.0,"    HELP");
    drawbutton(129,hmax-825,-279,hmax-925);
	glColor3f(0.0,0.0,0.99);
	distring(-0.17,-0.2,"CREDITS");
    drawbutton(130,hmax-970,-280,hmax-1080);
	glColor3f(0.0,0.0,0.99);
	distring(-0.16,-0.4,"    QUIT");
}
//Diplay Credits
void displaycredits()
{
    glLoadIdentity();
    drawbackground();
	glTranslatef(0.0f,0.0f,0.0f);
	glColor3f(0.5,0.1,1.0);
	distring(-0.5,0.1,"Made By:");
        glColor3f(1.0,0.1,0.5);
	distring(-0.5,-0.0,"Pradeep Pradhani(1OX13CS055)");
	glColor3f(1.0,0.1,0.5);
	distring(-0.5,-0.1,"Prashanth Jain P(1OX13CS056)");
	glColor3f(1.0,0.1,0.5);
	distring(-0.5,-0.2,"Shubham Kumar(1OX13CS076)");
	glColor3f(1.0,0.1,0.5);
	distring(-0.5,-0.3,"Vishnu Kanth S.K(1OX13CS090)");
    drawbutton(330,hmax-1050,-700,hmax-1150);
	glColor3f(0.0,0.1,0.7);
    distring(-0.4,-0.5,"BACK TO MAIN MENU");
}
//Display Help Screen
void displayhelp()
{
	glLoadIdentity();
    drawbackground();
	glTranslatef(0.0f,0.3f,0.0f);
	glColor3f(1.0,0.5,0.4);
	distring(-0.6,-0.1,"Are You Ready To Catch the Crazy Balls?");
	glColor3f(1.0,0.5,1.0);
	distring(-0.6,-0.2,"Use Arrow Keys OR A,S,W,D to Catch them");
	glColor3f(1.0,0.5,1.0);
	distring(-0.6,-0.3,"Make Sure You Catch them in the Black Zones");
	glColor3f(1.0,0.5,1.0);
	distring(-0.6,-0.4,"Inside the RED Box (Red Zone) You Will Loose the Game");
	glColor3f(1.0,0.5,1.0);
	distring(-0.6,-0.5,"You Get 60 seconds to Catch One");
	glColor3f(1.0,0.5,1.0);
	glTranslatef(0.0f,-0.3f,0.0f);
	drawbutton(330,hmax-1050,-700,hmax-1150);
	glColor3f(0.0,0.1,0.7);
    distring(-0.4,-0.5,"BACK TO MAIN MENU");
}
//Diplay the Win Screen
void nextlevel()
{
    drawbackground();
    char levels [50];
	if((int)time<TIME_MAX)
	sprintf(levels, "%d", (TIME_MAX-(int)time));
	else
	sprintf(levels, "0");
	displayLevel();
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,0.0f);
	glColor3f(1.0,0.0,1.0);
	distring(-0.5,0,"Wow,You Caught One!!");
	glColor3f(1.0,0.0,1.0);
	distring(-0.5,-0.1,"Point Earned: ");
	glColor3f(1.0,0.0,1.0);
	distring(0.1,-0.1,levels);
	glColor3f(1.0,0.5,1.0);
	distring(-0.5,-0.2,"Press Enter Catch Another One");
	glColor3f(1.0,0.0,0.5);
	distring(-0.5,-0.3,"Press x or X to Quit");
}
/*String Displaying Functions End */



/*Shape Drawing Function Begin */
//Draw button
void drawbutton(GLint x1,GLint y1,GLint x2,GLint y2)
{
    glLineWidth(2.0);
    glColor3f(onclickred,0.0,1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1/wmax,y1/hmax);
    glVertex2f(x1/wmax,y2/hmax);
    glColor3f(onclickred,0.0,1.0);
    glVertex2f(x2/wmax,y2/hmax);
    glVertex2f(x2/wmax,y1/hmax);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.60,0.60,1.00);
    glVertex2f((x1-20)/wmax,(y1-10)/hmax);
    glColor3f(0.60,0.60,1.00);
    glVertex2f((x1-20)/wmax,(y2+10)/hmax);
    glColor3f(0.60,0.60,1.00);
    glVertex2f((x2+20)/wmax,(y2+10)/hmax);
    glColor3f(0.60,0.60,1.00);
    glVertex2f((x2+20)/wmax,(y1-10)/hmax);
    glEnd();
    glColor3f(1.0,1.0,1.0);
}
//Draw Red Zone
void drawredzone()
{
	glLoadIdentity();
    glTranslated(-0.10,1.30,0.0);
	drawbackgroundball(0);
    glTranslated(0.10,-1.30,0.0);
	glColor4f(1.0,0.0,0.0,1.0);
	glBegin(GL_POLYGON);
	glVertex2f(rx1,ry1);
	glVertex2f(rx1,ry2);
	glVertex2f(rx2,ry2);
	glVertex2f(rx2,ry1);
	glEnd();

}
//Draw Ball A's
void draw(struct Ball O)
{
	glLoadIdentity();
	glTranslatef(O.X,O.Y,0.0f);
	//Using Triangle Fan for Drawing Circle
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(O.Red,O.Green,O.Blue);
	glVertex2f(0.0f,0.0f);
	int num=100,i;
	GLfloat angle;
	for(i=0;i<=num;i++)
	{
		angle=i*2.0f*PI/num;
		glVertex2f(cos(angle)*O.Radius,sin(angle)*O.Radius);
	}
	glEnd();
}
//Draw Ball B
void drawballB(struct Ball O)
{
	glLoadIdentity();
	glTranslatef(O.X,O.Y,0.0f);
	//Using Triangle Fan for Drawing Circle
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(O.Red,O.Green,O.Blue);
	glVertex2f(0.0f,0.0f);
	int num=100,i;
	GLfloat angle;
	for(i=0;i<=num;i++)
	{
		angle=i*2.0f*PI/num;
		glVertex2f(cos(angle)*O.Radius,sin(angle)*O.Radius);
	}
	glEnd();
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0,0.05);
	glVertex2f(-0.04,-0.03);
	glVertex2f(0.04,-0.03);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(0,-0.05);
	glVertex2f(-0.04,0.03);
	glVertex2f(0.04,0.03);
	glEnd();

}
/* Shape Drawing Func End */



/* Input Control Functions Begin */
//KeyBoard Function
void key(unsigned char c,int x,int y)
{
	if(screen==6 &&(c=='n'||c=='N'))
	{
        resetgame();
        screen=4;
	}
	if(c=='x'||c=='X')
	{
	exit(0);
	}
	if(c==13)
	{
	time=0;
	if(bm==0)
	screen=6;
	if(screen!=4 && screen!=5 && screen!=6)
	screen++;
	if(screen==5)
	screen=4;//Game Screen
	}
	if((c=='a'||c=='A' ||c==97)&&screen==4)
	{
		B.X-=B.XSpeed;
		if(B.X<clipAreaXLeft+B.Radius)
		B.X=clipAreaXLeft+B.Radius;
	}
	if((c=='d'||c=='D' ||c==39)&&screen==4)
	{

	B.X+=B.XSpeed;
	if(B.X>clipAreaXRight-B.Radius)
	B.X=clipAreaXRight-B.Radius;
	}
	if((c=='w'||c=='W'||c==38)&&screen==4)
	{
		B.Y+=B.YSpeed;
		if(B.Y > clipAreaYTop-B.Radius)
		B.Y=clipAreaYTop-B.Radius;
	}
	if((c=='s'||c=='S'||c==66)&&screen==4)
	{
	B.Y-=B.YSpeed;
	if(B.Y<clipAreaYBottom+B.Radius)
	B.Y=clipAreaYBottom+B.Radius;
	}
}
//Motion with ARROWS KEY
void keys(int c,int x,int y)
{
	if((c==GLUT_KEY_LEFT)&&screen==4)
	{

		B.X-=B.XSpeed;
		if(B.X<clipAreaXLeft+B.Radius)
		B.X=clipAreaXLeft+B.Radius;
	}
	if((c==GLUT_KEY_RIGHT)&&screen==4)
	{

	B.X+=B.XSpeed;
	if(B.X>clipAreaXRight-B.Radius)
	B.X=clipAreaXRight-B.Radius;
	}
	if((c==GLUT_KEY_UP)&&screen==4)
	{

		B.Y+=B.YSpeed;
		if(B.Y > clipAreaYTop-B.Radius)
		B.Y=clipAreaYTop-B.Radius;
	}
	if((c==GLUT_KEY_DOWN)&&screen==4)
	{
	B.Y-=B.YSpeed;
	if(B.Y<clipAreaYBottom+B.Radius)
	B.Y=clipAreaYBottom+B.Radius;
	}
}
//Mouse input
void mymouse(int btn,int state,int x,int y)
{
    int i;
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
      {
        if( screen==2 || screen==3 || screen==6)
        {
            printf("x:%d y:%d\n",x,y);
            if(x>645 && x<1060 && y>745 && y<820 )//check if in BACK TO MAIN MENU button area
                {
                 screen=1;
                 resetgame();
                 screen=1;
                }
        }
        else if(screen==1)
        {
	
	    if(x>815 && x<980 && y>375 && y<445 )//check if in PLAY button area
                 {
                    screen=4;
                    int i;
                    for(i=0;i<70000000;i++) ;//for 1 second delay
                 }
            if(x>815 && x<980 && y>485 && y<555 )//check if in HELP button area
                 screen=3;
            if(x>815 && x<980 && y>580 && y<655 )//check if in CREDITS button area
                 screen=2;
            if(x>815 && x<980 && y>680 && y<755 )//check if in EXIT button area
                exit(0);
        }
       }
}
/* Input Control Functions End */



/*Reshape Function for Reshaping The Image when Window Is Resized */
void reshape(GLsizei width,GLsizei height)
{
int i;
if(height ==0)
height=1;
GLfloat aspect=(GLfloat)width/(GLfloat)height;
glViewport(0,0,width,height);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
if(width>=height)
{
clipAreaXLeft=-1.0*aspect;
clipAreaXRight=1.0*aspect;
clipAreaYBottom=-1.0;
clipAreaYTop=1.0;
}
else
{
clipAreaXLeft=-1.0;
clipAreaXRight=1.0;
clipAreaYBottom=-1.0/aspect;
clipAreaYTop=1.0/aspect;
}
gluOrtho2D(clipAreaXLeft,clipAreaXRight,clipAreaYBottom,clipAreaYTop);
for(i=0;i<bm;i++)
{
A[i].XMin=clipAreaXLeft+A[i].Radius;
A[i].XMax=clipAreaXRight-A[i].Radius;
A[i].YMin=clipAreaYBottom+A[i].Radius;
A[i].YMax=clipAreaYTop-A[i].Radius;
}B.X=clipAreaXLeft+B.Radius;
B.Y=clipAreaYBottom+B.Radius;

}//reshape
/* Reshape Ends*/



/* AI Code Begin */
void moveA()
{
int i,j;
for(i=0;i<bm;i++)
{
//Move Balls
A[i].X+=A[i].XSpeed;
A[i].Y+=A[i].YSpeed;
//Ball Collison
	for(j=0;j<bm;j++)
	{
	if(i!=j)
	{
		if(collsion(A[i],A[j],0))
		{
			//printf("Collison Between %d and %d\n",i,j);

			//Collison Happened
			if((A[i].X+A[j].X)<=A[i].Radius)
			{
			A[i].XSpeed=-A[i].XSpeed;
			A[j].XSpeed=-A[j].XSpeed;
			}
			if((A[i].Y+A[j].Y)<=A[i].Radius)
			{
			A[i].YSpeed=-A[i].YSpeed;
			A[j].YSpeed=-A[j].YSpeed;
			}//if
		}//if
	}//if
	}//J
//Wall Collison
//Check Edge
	if(A[i].X>A[i].XMax)
	{	xflag[i]=0;
		A[i].X=A[i].XMax;
		A[i].XSpeed=-A[i].XSpeed;
	}
	else if(A[i].X <A[i].XMin)
	{	xflag[i]=0;
		A[i].X=A[i].XMin;
		A[i].XSpeed=-A[i].XSpeed;

	}
	if(A[i].Y>A[i].YMax)
	{	yflag[i]=0;
		A[i].Y=A[i].YMax;
		A[i].YSpeed=-A[i].YSpeed;

	}
	else if(A[i].Y <A[i].YMin)
	{	yflag[i]=0;
		A[i].Y=A[i].YMin;
		A[i].YSpeed=-A[i].YSpeed;
	}
}//I
}//func
/*AI code Ends */



/*Display Function to Display the Images  Begins*/
void display()
{
	int i;
	glClearColor(1,1,1,0.4);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	if(screen==1)
	{
        displaywelcome();
	}
	else if(screen==2)
	{
        displaycredits();
	}
	else if(screen==3)
	{
        displayhelp();
	}
	else if(screen==5)
	{
        nextlevel();
	}
	else if(screen==4)
    {
        glTranslatef(2.0,+1.0,0.0);
        drawbackground();
        glTranslatef(-2.0,-1.0,0.0);
        drawredzone();
        for(i=0;i<bm;i++)
            draw(A[i]);
        drawballB(B);
        displayLevel();
        displayScore();
        displaytimeleft();
        moveA();
	}
	else if(screen==6)
	{
        gameover();
	}
	else
	{
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,0.0f);
	glColor3f(1.0,0.0,1.0);
	distring(-0.5,0,"Some Eror Occured");
	}
	glutSwapBuffers();
}//Display End
/*Display Function to Display the Images Ends*/



/*Timer Function to Refresh Screen It Also Check for Win or Lost also  Begin*/
void Timer(int value)
{
int i;
if(screen==4)
time+=0.05;
if((int)time>TIME_MAX)
screen=6;
//Check Won OR Lost
for(i=0;i<bm;i++)
if(collsion(A[i],B,0))
{
if(!inredzone(B))
{
	initGL();
	if(A[i].XSpeed>0.0)
	A[i].XSpeed*=factor;
	else
	{
	A[i].XSpeed*=-1.0f;
	A[i].XSpeed*=factor;
	}
	if(A[i].YSpeed>0.0)
	A[i].YSpeed*=factor;
	else
	{
	A[i].YSpeed*=-1.0f;
	A[i].YSpeed*=factor;
	}
	level++;
	bm--;
	if((int)time<TIME_MAX)
	score+=(TIME_MAX-(int)time);
	else
	screen=6;//Lost Screen
	screen=5;//5 WON Screen
}
else
screen=6;
}
glutPostRedisplay();
glutTimerFunc(refreshMillis,Timer,0);
}//timer
/*Timer Function Ends */



/*Main Function*/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	initGL();
	glutInitDisplayMode(GLUT_RGB | GLUT_ALPHA);
	glutInitWindowSize(1920,1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("The Crazy Balls");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,Timer,0);
	glutKeyboardFunc(key);
	glutSpecialFunc(keys);
	glutMouseFunc(mymouse);
	glutMainLoop();
	return 1;
}

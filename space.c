#include <stdlib.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <math.h>
#include <stdio.h>

// ブロック数
#define	I	400
// 敵数
#define	J	400
#define RAD (M_PI / 180.0)
//ポイント点
#define	P	300
//弾の個数
#define	G	300
//変数ブロック
int k=0;
// 自機の位置
typedef struct {
  float ShiftX;     
  float ShiftY;
  float ShiftZ;
  float RotateX;
  float RotateY;
  float RotateZ;
}Geometry;
Geometry Player;

// フラグなどの宣言と初期化
char  AnimationFlag = 0;
int   xBegin, yBegin;
int   PressButton;
int   i=0,m=0,n=0;
int   jump = 0;
char  left = 0;
char	right = 0;
char  up = 0;
char  down = 0;
char  back = 0;
char  front = 0;
char		direction	=	0;
char		situation	=	99;
int		limit_left[I] ={1};
int		limit_right[I] ={1};
int		limit_up[I] ={1};
int		limit_down[I] ={1};
int		limit_back[I] ={1};
int		limit_front[I] ={1};
int		t_limit_left	=	1;
int		t_limit_right	=	1;
int		t_limit_up	=	1;
int		t_limit_down	=	1;
int		t_limit_back	=	1;
int		t_limit_front	=	1;
int		fall=1,fall_p=0;
int		rise=1,rise_p=0;
double          Rotate=0;
double		mini=0,max=100;
double    Cv=0.1;
double		Vy=0.1;
double		Vz=0.1;

double gan=0;
double gans=0.05;
double gan_m=0;

int winWidth  = 1700;
int winHeight = 900;

// 時間変数の宣言と初期化
double		ntime=0,ptime=0;
double		die_ntime=0,die_ptime=0;

// ポイント得点
int point=0;

// ブロックと敵の宣言と初期化
double		block_x[I];
double		block_y[I];
double		block_z[I];
double		enemy_x[J]={0};	
double		enemy_y[J]={0};	
double		enemy_z[J]={0};	
double		enemy_x2[J]={0};
double		enemy_y2[J]={0};	
double		enemy_z2[J]={0};

double    gole_x[P]={0};
double    gole_y[P]={0};
double    gole_z[P]={0};

double    gan_x[G]={0};
double    gan_y[G]={0};
double    gan_z[G]={0};


double		bL[I],bR[I],bU[I],bD[I],bF[I],bB[I],R[J],R2[J],R3[P];
double		nView[2];
int		jcount=0;
float CameraAzimuth   = 90.0;   
float CameraElevation = 0.0;    
float CameraDistance  = 5.0;   
float CameraX = 0.0;           
float CameraY = 0.0;          
float CameraZ = 5.0;           


void display_window1(int x, int y, int w, int h);
void display_window2(int x, int y, int w, int h);


void drawString3D(const char *str, float charSize, float lineWidth)
{
  glPushMatrix();
  glPushAttrib(GL_LINE_BIT);
  glScaled(0.001 * charSize, 0.001 * charSize, 0.01);
  glLineWidth(lineWidth);
  while(*str){
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
    ++str;
  }
  glPopAttrib();
  glPopMatrix();
}

void display(void)
{
  if(k==0){
    for(i=0;i<I;i++){
        block_x[i]	=	rand()%30+1;
        block_y[i]	=	-10+rand()%40;
        block_z[i]	=	-40+rand()%80;
        k=k+1;

        limit_left[i] =1;
  		limit_right[i] =1;
	   	limit_up[i] =1;
  		limit_down[i] =1;
    }
  }
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             
    glLoadIdentity();              
    display_window1(0, 0, 1200 , 900);
    display_window2( 1200,600, 500, 300);               
    glutSwapBuffers();
  
}

void display_window1(int x, int y, int w, int h)
{
glPushMatrix ();

  glViewport(x, y, w, h); 
  glMatrixMode(GL_MODELVIEW);            
  glLoadIdentity();    

  gluLookAt(  Player.ShiftX-0.7, Player.ShiftY+0.6, Player.ShiftZ,  
	      Player.ShiftX+3, Player.ShiftY+0.5, Player.ShiftZ, 
	      0.0, 1.0, 0.0);
  
  glEnable(GL_DEPTH_TEST);    
 
  if(situation==99){
    glPushMatrix ();
    glColor3f( 0.0, 0.0, 1.0 );  
    glTranslatef(2.2, 6.0, -3.0); 
    glRotated(-90.0,0.0,1.0,0.0);
    drawString3D("Space Game Start", 6.0, 2.0); 
    glPopMatrix ();   
  }

    if(situation==12){
    glPushMatrix ();
    glColor3f( 1.0, 1.0, 1.0 );  
    glTranslatef(Player.ShiftX+1.3, Player.ShiftY+1.7, Player.ShiftZ-1.8); 
    glRotated(-90.0,0.0,1.0,0.0);
    drawString3D("G a m e", 6, 5.0); 
    glPopMatrix ();   
    glPushMatrix ();
    glColor3f( 1.0, 1.0, 1.0 );  
    glTranslatef(Player.ShiftX+1.3, Player.ShiftY, Player.ShiftZ-1.8); 
    glRotated(-90.0,0.0,1.0,0.0);
    drawString3D("C l e a r", 6, 5.0); 
    glPopMatrix ();   
  }
  
  // 自機の描画
  glPushMatrix (); 
  glTranslatef(Player.ShiftX,Player.ShiftY,Player.ShiftZ);
  glRotatef(ptime*100,1.0,0.0,0.0);
  if(situation==2){
    glColor3f( 1.0-die_ptime, 1.0-die_ptime, 1.0 ); 
  }
  else if(situation==0){
    glColor3f( 1.0, 1.0-die_ptime, 1.0-die_ptime ); 	  
  }
  else{
    glColor3f( 1.0, 1.0, 1.0 );   
  }
  glRotatef(90,0.0,1.0,0.0);
  glutWireCone(0.2,1,20,10);
  glTranslatef(-0.2,0,0.2);
  glRotatef(180,0.0,1.0,0.0);
  glutWireCone(0.1,0.8,10,2);
  glTranslatef(-0.4,0,0);
  glutWireCone(0.1,0.8,10,2);
  glPopMatrix ();   
  
  // 初期位置の床
  glPushMatrix (); 
  glColor3f( 0.0,1.0,0.0 ); 
  glTranslatef(0.0,4,0.0);
  glScalef(1.0,0.1,1.0);
  if(Player.ShiftX<0.1 && -0.1<Player.ShiftX){
    glutSolidCube(1.0);
  }
  else{
    glutWireCube(1.0);
  }
  glPopMatrix ();
  
  
  // 敵の描画
  for(i=0;i<J;i++){
    glPushMatrix ();   
    glColor3f( 0.0,0.0,1.0 ); 
    glRotatef(-90,0.0,1.0,0.0);
    glTranslatef(enemy_z[i],enemy_y[i],enemy_x[i]);
    glRotatef(ptime*100,0.0,0.0,1.0);
    glutWireCone(0.25,0.5,10,2);
    glPopMatrix ();
    
    glPushMatrix ();   
    glColor3f( 0.2,0.4,0.2 ); 
    glTranslatef(enemy_x2[i],enemy_y2[i],enemy_z2[i]);
	  glRotatef(ptime*100,0.0,0.0,1.0);
    glutSolidSphere(0.4,10.0,4.0);
    glColor3f( 1.0,1.0,0.0 ); 
    glutWireSphere (0.4,10.0,4.0);
	glPopMatrix ();
  }
  
  //ポイントの描画
  for(i=0;i<P;i++){
    glPushMatrix ();   
    glColor3f( 1.0,1.0,1.0 ); 
    glTranslatef(gole_x[i],gole_y[i],gole_z[i]);
    glRotatef(ptime*100,0.0,1.0,0.0);
    glutWireTorus(0.1,0.3,30,30);
    glPopMatrix ();
  }

  // ブロックの描画
  for(i=0;i<I;i++){
    glPushMatrix (); 
    glTranslatef(block_x[i],block_y[i],block_z[i]);
    if(i!=I-1){	 
      glColor3f( 0.50, 0.25, 0.25 ); 
    }
    else{
      glColor3f( 0.5, 1.0, 0.5 ); 
    }glutSolidCube(1.0);
    glColor3f( 1.0, 1.0, 0.0 ); 
    glutWireCube(1.0);
    glPopMatrix (); 
  }

  //弾の描写
    if(gan==1){
    glPushMatrix ();   
    glColor3f( 1.0,1.0,1.0 ); 
    glTranslatef(gan_x[0],gan_y[0],gan_z[0]);
    glutSolidSphere(0.06,10.0,10.0);
    gan_m+=1;
    gan_x[0]+=gans;
    if(gan_m==300){
      gan=0;
      gan_m=0;
    }
    glPopMatrix ();
    }
  glDisable(GL_DEPTH_TEST);
  glPopMatrix ();
}


//視点以外は基本機には１と同様
void display_window2(int x, int y, int w, int h)
{
glPushMatrix ();

  glViewport(x, y, w, h);
  glMatrixMode(GL_MODELVIEW);             
  glLoadIdentity();    

  //自由視点
  gluLookAt(  Player.ShiftX+CameraX+1, Player.ShiftY+CameraY+3, Player.ShiftZ+CameraZ,  
	      Player.ShiftX+1, Player.ShiftY+3, Player.ShiftZ,  
	      0.0, 1.0, 0.0); 
  glEnable(GL_DEPTH_TEST);    


  glPushMatrix (); 
  glTranslatef(Player.ShiftX,Player.ShiftY,Player.ShiftZ);
  glRotatef(ptime*100,1.0,0.0,0.0);
  if(situation==2){
    glColor3f( 1.0-die_ptime, 1.0-die_ptime, 1.0 ); 
  }
  else if(situation==0){
    glColor3f( 1.0, 1.0-die_ptime, 1.0-die_ptime ); 	  
  }
  else{
    glColor3f( 1.0, 1.0, 1.0 );   
  }
  glRotatef(90,0.0,1.0,0.0);
  glutWireCone(0.2,1,20,10);
  glTranslatef(-0.2,0,0.2);
  glRotatef(180,0.0,1.0,0.0);
  glutWireCone(0.1,0.8,10,2);
  glTranslatef(-0.4,0,0);
  glutWireCone(0.1,0.8,10,2);
  glPopMatrix ();   
  
  glPushMatrix (); 
  glColor3f( 0.0,1.0,0.0 ); 
  glTranslatef(0.0,4,0.0);
  glScalef(1.0,0.1,1.0);
  if(Player.ShiftX<0.1 && -0.1<Player.ShiftX){
    glutSolidCube(1.0);
  }
  else{
    glutWireCube(1.0);
  }
  glPopMatrix ();
  
  
  for(i=0;i<J;i++){
    glPushMatrix ();   
    glColor3f( 0.0,0.0,1.0 ); 
    glRotatef(-90,0.0,1.0,0.0);
    glTranslatef(enemy_z[i],enemy_y[i],enemy_x[i]);
    glRotatef(ptime*100,0.0,0.0,1.0);
    glutWireCone(0.25,0.5,10,2);
    glPopMatrix ();
    
    glPushMatrix ();   
    glColor3f( 0.2,0.4,0.2 ); 
    glTranslatef(enemy_x2[i],enemy_y2[i],enemy_z2[i]);
	  glRotatef(ptime*100,0.0,0.0,1.0);
    glutSolidSphere(0.4,10.0,4.0);
    glColor3f( 1.0,1.0,0.0 ); 
    glutWireSphere (0.4,10.0,4.0);
	glPopMatrix ();
  }
  
  for(i=0;i<P;i++){
    glPushMatrix ();   
    glColor3f( 1.0,1.0,1.0 ); 
    glTranslatef(gole_x[i],gole_y[i],gole_z[i]);
    glRotatef(ptime*100,0.0,1.0,0.0);
    glutWireTorus(0.1,0.3,30,30);
    glPopMatrix ();
  }

  for(i=0;i<I;i++){
    glPushMatrix (); 
    glTranslatef(block_x[i],block_y[i],block_z[i]);
    if(i!=I-1){	 
      glColor3f( 0.50, 0.25, 0.25 ); 
    }
    else{
      glColor3f( 0.5, 1.0, 0.5 ); 
    }glutSolidCube(1.0);
    glColor3f( 1.0, 1.0, 0.0 ); 
    glutWireCube(1.0);
    glPopMatrix (); 
  }
    if(gan==1){
    glPushMatrix ();   
    glColor3f( 1.0,1.0,1.0 ); 
    glTranslatef(gan_x[0],gan_y[0],gan_z[0]);
    glutSolidSphere(0.06,10.0,10.0);
    gan_m+=1;
    gan_x[0]+=gans;
    if(gan_m==300){
      gan=0;
      gan_m=0;
    }
    glPopMatrix ();
    }
  glDisable(GL_DEPTH_TEST);
  glPopMatrix ();
}

void timer(int value)
 {
   
   switch	(situation)	{
     
     // 通常時
   case	1:
     
     // ブロックとの当たり判定
     for(i=0;i<I;i++){
       
       // 左右上下の判定用
       bL[i]	=	block_x[i]-Player.ShiftX;
       bR[i]	=	Player.ShiftX-block_x[i];
       bU[i]	=	Player.ShiftY-block_y[i];
       bD[i]	=	block_y[i]-Player.ShiftY;
       bB[i]  = block_z[i]-Player.ShiftZ;
       bF[i]  = Player.ShiftZ-block_z[i];
       
            // 左壁
       if(bL[i]<1 && -0.5<bL[i] && bU[i]<1 && bD[i]<1 && bB[i]<1 && bF[i]<1){
	 limit_right[i]	=	0;
   Player.ShiftX-=0.5;
       }
       else{
	 limit_right[i]	=	1;
	 t_limit_right		=	1;
       }		   
 
            // 右壁
       if(-0.5<bR[i] && bR[i]<1 && bU[i]<1 && bD[i]<1 && bB[i]<1 && bF[i]<1){
	 limit_left[i]	=	0;
   Player.ShiftX+=0.5;
       }
       else{
	 limit_left[i]	=	1;
	 t_limit_left		=	1;
       }
            // 上壁
       if(-0.5<bU[i] && bU[i]<1 && bL[i]<1 && bR[i]<1 && bB[i]<1 && bF[i]<1){
	 limit_up[i]	=	0;
   Player.ShiftY+=0.5;
       }
       else{
	 limit_up[i]	=	1;
	 t_limit_up 	=	1;
       }

            // 下壁
       if(-0.5<bD[i] && bD[i]<1 && bR[i]<1 && bL[i]<1 && bB[i]<1 && bF[i]<1){
	 limit_down[i]	=	0;
   Player.ShiftY-=0.5;
       }
       else{
	 limit_down[i]	=	1;
	 t_limit_down		=	1;
       }     
            // 奥壁
       if(-0.5<bB[i] && bB[i]<1 && bU[i]<1 && bD[i]<1 && bL[i]<1 && bR[i]<1){
	 limit_back[i]	=	0;
   Player.ShiftZ-=0.5;
       }
       else{
	 limit_back[i]	=	1;
	 t_limit_back		=	1;
       }
            // 前壁
       if(-0.5<bF[i] && bF[i]<1 && bU[i]<1 && bD[i]<1 && bL[i]<1 && bR[i]<1){
	 limit_front[i]	=	0;
   Player.ShiftZ+=0.5;
       }
       else{
	 limit_front[i]	=	1;
	 t_limit_front		=	1;
       }

     }

     // 移動フラグ
     for(i=0;i<I;i++){	
       t_limit_right	*=	limit_right[i];				
     }
     for(i=0;i<I;i++){	
       t_limit_left	*=	limit_left[i];				
     }

     for(i=0;i<I;i++){	
       t_limit_up	*=	limit_up[i];				
     }

     for(i=0;i<I;i++){	
       t_limit_down	*=	limit_down[i];				
     }

    for(i=0;i<I;i++){	
       t_limit_back	*=	limit_back[i];				
     }

     for(i=0;i<I;i++){	
       t_limit_front	*=	limit_front[i];				
     }
    

     // 自機の回転
     if(direction==0){
       Rotate-= 1;
     }
     else{
       Rotate+= 1;	
     }
     if( Player.RotateZ > 360.0 ){	  
       Player.RotateZ -= 360.0;
     }
     
     // 自機の異動(右)
     if(right==1 && t_limit_right == 1){
       Player.ShiftX+=Cv;
       direction=0;
     }
     
     // 自機の異動(左)
     if(left==1 && t_limit_left == 1){
       Player.ShiftX-=Cv;
       direction=1;
     }
          // 自機の異動(上)
     if(up==1 && t_limit_up == 1){
       Player.ShiftY+=Vy;
       direction=1;
     }
          // 自機の異動(下)
     if(down==1 && t_limit_down == 1){
       Player.ShiftY-=Vy;
       direction=1;
     }

         // 自機の異動(上)
     if(back==1 && t_limit_back == 1){
       Player.ShiftZ-=Vz;
       direction=1;
     }
          // 自機の異動(下)
     if(front==1 && t_limit_front== 1){
       Player.ShiftZ+=Vz;
       direction=1;
     }
     
		  
     // 敵との当たり判定
     for(i=0;i<J;i++){
       R[i]	=	sqrt(pow(Player.ShiftX+enemy_x[i],2.0)
            +pow(Player.ShiftY-enemy_y[i],2.0)
            +pow(Player.ShiftZ-enemy_z[i],2.0));
       if(R[i]<0.5){
	 situation=0;
       }
       R2[i]	=	sqrt(pow(Player.ShiftX-enemy_x2[i],2.0)
			     +pow(Player.ShiftY-enemy_y2[i],2.0)
			     +pow(Player.ShiftZ-enemy_z2[i],2.0));
       if(R2[i]<0.5){
	 situation=0;
		    }
     }

          for(i=0;i<J;i++){
       R[i]	=	sqrt(pow(gan_x[0]+enemy_x[i],2.0)
            +pow(gan_y[0]-enemy_y[i],2.0)
            +pow(gan_z[0]-enemy_z[i],2.0));
       if(R[i]<0.5){
	      enemy_x[i]=1000;
        gan=0;
       }
       R2[i]	=	sqrt(pow(gan_x[0]-enemy_x2[i],2.0)
			     +pow(gan_y[0]-enemy_y2[i],2.0)
			     +pow(gan_z[0]-enemy_z2[i],2.0));
       if(R2[i]<0.5){
	      enemy_x2[i]=1000;
        gan=0;
		    }
     }

     //ポイントおん当たり判定
     for(i=0;i<P;i++){
       R3[i]	=	sqrt(pow(Player.ShiftX-gole_x[i],2.0)
          +pow(Player.ShiftY-gole_y[i],2.0)
          +pow(Player.ShiftZ-gole_z[i],2.0));
       if(R3[i]<0.5){
         point=point+1;
         gole_x[i]=1000;
         if(point==2){
                     for(i=0;i<J;i++){
                enemy_x[i]	=	1000;
                }
           for(i=0;i<J;i++){
                enemy_x2[i]	=1000;
                }
           for(i=0;i<I;i++){
                block_x[i]  =1000;
                }
           for(i=0;i<P;i++){
                 gole_x[i]  =1000;
                }
	 situation=12;
         }
   printf("%d",point);
       }
     }
     
     // 敵の移動
     for(i=0;i<J;i++){
       enemy_x[i]	+=	0.1;
       enemy_z2[i]	+=	0.1;
       if(enemy_x[i]>-5){
	 enemy_y[i]	=	-10+rand()%40;	
	 enemy_x[i]	=	enemy_x[i]-30;	
       }
       if(enemy_z2[i]>80){
	 enemy_y2[i]	=	-20+rand()%40;	
	 enemy_x2[i]	=	5+rand()%30;	
	 enemy_z2[i]	=  -40+rand()%80;
		    }
     }
      
      //ポイントの移動
      for(i=0;i<P;i++){
       gole_z[i]	+=	0.000000001;
     if(gole_z[i]>80){
	 gole_y[i]	=	-20+rand()%40;
	 gole_x[i]	=	5+rand()%15;	
   gole_z[i]	=  -40+rand()%80;
       }
     }

     // 時間変数の更新
     ntime	=	ntime-(ntime-0.1);
     ptime	+=	ntime;

     break;
     
     // 死亡時とクリア時
   case	0:
   case	2:
     ntime	=	ntime-(ntime-0.1);
     ptime	+=	ntime;
     die_ntime	=	die_ntime-(die_ntime-0.005);
     die_ptime	+=	die_ntime;
     if(die_ptime>1){
       exit(0);
     }		
     break;
   }
   
   
   glutPostRedisplay();
   glutTimerFunc(3 , timer , 0);
   
   
 }


void keyboard1(int key, int x, int y )
{
  switch( key ) {

  case GLUT_KEY_UP:
		up	=	1;
    break;

  case GLUT_KEY_DOWN:
		down =	1;
    break;

  case GLUT_KEY_RIGHT:
		front = 1;
    break;   

  case GLUT_KEY_LEFT:
		back = 1;
    break;

  case 's':
    situation=1;
    break;
  }
  glutPostRedisplay();
  
  x = y = 0;
} 
void keyboard2(int key, int x, int y )
{
  switch( key ) {

  case ' ':
    gan_x[0]=Player.ShiftX+1;
    gan_y[0]=Player.ShiftY;
    gan_z[0]=Player.ShiftZ;
    gan=1;
    break;

  case GLUT_KEY_UP:
		up	=	0;
    break;       

  case GLUT_KEY_DOWN:
		down	=	0;
    break;

  case GLUT_KEY_RIGHT:
		front	=	0;
    break;   

  case GLUT_KEY_LEFT:
		back	=	0;
    break;    
  
  }
  
  glutPostRedisplay();
  
  x = y = 0;
} 

void mouseButton(int button, int state, int x, int y )
{
  if (state == GLUT_DOWN)
  {
    switch(button)
    {

      // 左クリックで左移動
    case GLUT_LEFT_BUTTON:
	    PressButton = 0;
		left=1;
      break;
      // 真中クリックで視点変更
    case GLUT_MIDDLE_BUTTON:
      PressButton = button;
      break;
    
      // 右クリックで右移動
    case GLUT_RIGHT_BUTTON:
	    PressButton = 0;
		right=1;
	  break;
    }
 
    xBegin = x;
    yBegin = y;
  }
  else{
	right=0;
	left=0;
  }
}

void mouseDrag(int x, int y)
{
  int xMove = x - xBegin;
  int yMove = y - yBegin;
 
  switch (PressButton) {

  case GLUT_MIDDLE_BUTTON:
      CameraAzimuth   += (float)xMove / 2.0;
      CameraElevation += (float)yMove / 2.0;
      if (CameraElevation >  90.0){
	CameraElevation =  90.0;
	    }
      if (CameraElevation < -90.0){
	CameraElevation = -90.0;	
      }
    break;
    
  }
  
  CameraX = CameraDistance * cos(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
  CameraY = CameraDistance * sin(CameraElevation * RAD);
  CameraZ = CameraDistance * sin(CameraAzimuth * RAD) * cos(CameraElevation * RAD); 
  
  xBegin = x;
  yBegin = y;
  
  glutPostRedisplay();
}

void myInit (char *windowTitle)
{

  float aspect = (float)winWidth / (float)winHeight;

  glutInitWindowPosition(0, 0);                 
  glutInitWindowSize(winWidth, winHeight);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
  glutCreateWindow(windowTitle); 
  glClearColor (0.0, 0.0, 0.0, 1.0);

  glutSpecialFunc(keyboard1);  
  glutSpecialUpFunc(keyboard2);
  glutMouseFunc   (mouseButton); 
  glutMotionFunc  (mouseDrag); 
  glutDisplayFunc (display);
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity(); 
  gluPerspective(90.0, aspect, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glutTimerFunc(15 , timer , 0);
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  myInit(argv[0]);

  // 自機位置の初期化
  Player.ShiftX = 0;
  Player.ShiftY = 4.5;
  Player.ShiftZ = 0;
  Player.RotateX = 0;
  Player.RotateY = 0;
  Player.RotateZ = 0;  


  
  // 敵位置(青)の初期化
  for(i=0;i<J;i++){
	enemy_x[i]	=	rand()%40;
	enemy_y[i]	=	rand()%30;
  enemy_z[i]	=	-25+rand()%50;
  }

  // 敵位置(緑)の初期化
  for(i=0;i<J;i++){
	enemy_z2[i]	=	-40+rand()%80;
	enemy_x2[i]	=	10+rand()%30;
	enemy_y2[i]	=	-10+rand()%40;
  }

  //ポイントの初期化
  for(i=0;i<P;i++){
	gole_x[i]	=	10+rand()%30;
	gole_y[i] =	-10+rand()%40;
	gole_z[i]	=	-40+rand()%80;
  }

  glutMainLoop(); 
  
  return( 0 );
}
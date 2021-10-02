#include<FL/Fl.H>
#include<FL/Fl_Widget.H>
#include<FL/Fl_Box.H>
#include<FL/fl_draw.H>
#include<iostream>
#include<stdlib.h>
#include<cmath>
#include<string>
#include<string.h>
#include<cstdlib>
#include<stdio.h>
#include<cstring>

#define WINX 800
#define WINY 800
#define KEY_UP 65362
#define KEY_DOWN 65364
#define KEY_LEFT 65361
#define KEY_RIGHT 65363
#define PI 3.1415
#define MARGX 30
#define MARGY 50
#define OBSW 40
#define OBSH 40
using namespace std;

void setScore(Fl_Box * sbptr, int score){
	string s = "Score: " + to_string(score);
	char * t = new char[1000];
	strcpy(t, s.c_str());
	sbptr->label(t);
}
char *int2charstar (int v) {
	char *s = new char[1000];
	sprintf(s,"Score: %d",v);
	return s;
}
class Shape : public Fl_Widget  {
  	protected:
	int x, y, w, h;
	public:
		Shape (int x, int y, int w, int h) : Fl_Widget (x,y,w,h){
			this->x=x; this->y=y;
			this->w=w; this->h=h;
		}
		virtual void wipe()=0;
		// virtual void refresh(int dx=0, int dy=0)=0;
        virtual void newDraw(int color=-1)=0;
        virtual void resetPosition(int x,int y){
            wipe();
            this->x = x;
            this->y = y;
            resize(x,y,w,h);
            newDraw();
        }
        virtual int getX(){return x;}
        virtual int getY(){return y;}
        virtual int getW(){return w;}
        virtual int getH(){return h;}
};
class ActiveShape:public Shape{
    protected:
        Shape* playerptr;
    public:
        ActiveShape(int x, int y, int w, int h):Shape(x,y,w,h){

        }
        virtual void setPlayerPtr(Shape *ptr){
            playerptr = ptr;
        } 
        virtual bool getContact(int pcx, int pcy, int pw, int ph){
            int mcx, mcy;
            mcx = x+0.5*w; mcy = y+0.5*h;
            bool incontact = false;
            if((pcx-0.5*pw<x+w&&pcx-0.5*pw>x)||(pcx+0.5*pw<x+w&&pcx+0.5*pw>x)){
                if((pcy-0.5*ph<y+h&&pcy-0.5*ph>y)||(pcy+0.5*ph<y+h&&pcy+0.5*ph>y)){
                    incontact = true;
                }
            }
            if(!incontact){
                if(sqrt((mcx-pcx)*(mcx-pcx)+(mcy-pcy)*(mcy-pcy))<pw+w){
                    incontact=true;
                }
            }
            return incontact;
        }

        virtual bool checkPlayerDist(){
            // playerptr = playerptr;
            int px=playerptr->getX();
            int py=playerptr->getY();
            int pw=playerptr->getW();
            int ph=playerptr->getH();
            int pcx = px+0.5*pw;
            int pcy = py+0.5*ph;
            cout<<pcx<<" "<<pcy<<" "<<x<<" "<<y<<" "<<pw<<" "<<ph<<"\n";
            bool incontact = getContact(pcx, pcy, pw, ph);
            if(incontact){
                // std::cout<<"Trouble\n";
                return true;
            }
            return false;
        }
        
};
class MovingShape:public Shape{
    protected:
        double vx,vy,ax,ay;
        double vmin, vmax;
        bool randomizeMotion;
    public:
        MovingShape(int x, int y, int w, int h):Shape(x,y,w,h){};
        virtual void setMotionVariables(double _vx, double _vy, double _ax, double _ay, bool _rand){
            vx=_vx;vy=_vy;
            ax=_ax;ay=_ay;
            vmin=10;vmax=40;
            randomizeMotion=_rand;
        }
        // virtual void refresh(int dx, int dy){
        //     wipe();
        //     resize(x, y, w, h);
        //     newDraw();
        // }
        virtual void nextStep(double dt){
            wipe();
            // std::cout<<sqrt(vx*vx+vy*vy)<<"\n";
            x+=vx*dt;y+=vy*dt;
            if(randomizeMotion){
                int theta = rand()%120- 60;
                double c = cos(PI*theta/180), s = sin(PI*theta/180);
                double ux,uy;
                // ux = vx*c + vy*s;
                // uy = vx*c - vy*s;
                ux = sqrt(vx*vx+vy*vy)*c;
                uy = sqrt(vx*vx+vy*vy)*s;
                vx = ux; vy = uy;
            }
            else{
                vx+=ax*dt;vy+=ay*dt;
            }
            if(x+w>=WINX-MARGX){vx=-abs(vx);}
            if(x<MARGX){vx=abs(vx);}
            if(y+h>=WINY-MARGY){vy=-abs(vy);}
            if(y<MARGY){vy=abs(vy);}            
            resize(x,y,w,h);
            newDraw();

        };
        virtual void draw(){
            newDraw();
        }
};
class MyRectangle : public MovingShape {
 protected:
    int mycolor;
 public:
   MyRectangle (int x,int y,int w, int h) : MovingShape (x,y,w,h) {
       mycolor = 120;
   }
   virtual void newDraw(int color=-1) {
       if(color==-1){color=mycolor;}
	fl_draw_box (FL_FLAT_BOX,x,y,w,h,color);
   }
   virtual void wipe() {
        newDraw(FL_WHITE);
   }
//    virtual void refresh(int dx, int dy) {
// 	x=x+dx;y=y+dy;
// 	resize(x,y,w,h);
// 	fl_draw_box (FL_FLAT_BOX,x,y,w,h,120);
//    }

};
class MyEllipse : public MovingShape {
 protected:
    int mycolor;
 public:
   MyEllipse (int x,int y,int w, int h) : MovingShape (x,y,w,h) {
       mycolor = FL_GREEN;
   }
   virtual void newDraw(int color=-1) {
        if(color==-1){
            color=mycolor;
            // std::cout<<color;
        }
		fl_color(color);
	    fl_pie (x,y,w,h,0,360);
   }
   virtual void wipe() {
    //    std::cout<<FL_WHITE;
        newDraw(FL_WHITE);
   }
//    virtual void refresh(int dx, int dy) {
// 	x=x+dx;y=y+dy;
// 	resize(x,y,w,h);
// 	fl_draw_box (FL_FLAT_BOX,x,y,w,h,120);
//    }

};
class PlayerGon: public MyEllipse{
    Fl_Box *sbptr;
    int score;
    public:
        PlayerGon(int x, int y, int w, int h): MyEllipse(x,y,w,h){
            score= 0;
        }
        virtual void upKeyTask(){
            if(w>200||h>200){
                return;
            }
            wipe();
            x = x-0.05*w;
            // std::cout<<x<<"\n";
            w = 1.1*w;
            y = y-0.05*w;
            h = 1.1*h;
            resize(x,y,w,h);
            newDraw();
        }
        virtual void downKeyTask(){
            if(w<50||h<50){
                return;
            }
            wipe();
            x = x+0.05*w;
            w = 0.9*w;
            y = y+0.05*w;
            h = 0.9*h;
            resize(x,y,w,h);
            newDraw();
        }
        virtual void mouseMoveTask(int mx, int my){
            wipe();
            x = mx-0.5*w;y = my-0.5*h;
            resize(x,y,w,h);
            newDraw();
            increment(w/50);
        }
        virtual int handle(int e){
            if(e==FL_KEYDOWN){
                int cmd = Fl::event_key();
                if(cmd==KEY_UP){
                    upKeyTask();
                }
                else if(cmd==KEY_DOWN){
                    downKeyTask();
                }
            }
            else if(e==FL_MOVE){
                int mx = Fl::event_x(), my = Fl::event_y();
                mouseMoveTask(mx, my);
                
                // std::cout<<Fl::event_x()<<" "<<Fl::event_y()<<"\n";
            }
            return 1;
        }
        void failGame();
        void setScoreBoard(Fl_Box *sbptr){
            this->sbptr = sbptr;
        }
        void increment(int points=1){
            score+=points;
            setScore(sbptr, score);
        }
};
class ObstacleRect: public MyRectangle{
    PlayerGon *playerptr;
    public:
        ObstacleRect(int x, int y, int w, int h):MyRectangle(x,y,w,h){

        }
        void setPlayerPtr(PlayerGon *ptr){
            playerptr = ptr;
        }
        bool getContact(int pcx, int pcy, int pw, int ph){
            int mcx, mcy;
            mcx = x + 0.5 * w; mcy = y + 0.5 * h;
            bool incontact = false;
            if ((pcx - 0.5 * pw<x + w && pcx - 0.5 * pw>x) || (pcx + 0.5 * pw<x + w && pcx + 0.5 * pw>x)) {
                if ((pcy - 0.5 * ph<y + h && pcy - 0.5 * ph>y) || (pcy + 0.5 * ph<y + h && pcy + 0.5 * ph>y)) {
                    incontact = true;
                }
            }
            if (!incontact) {
                if (sqrt((mcx - pcx) * (mcx - pcx) + (mcy - pcy) * (mcy - pcy)) < 0.5 * (pw + w)) {
                    incontact = true;
                }
            }
            return incontact;
        }
        void checkPlayerDist(){
            int px=playerptr->getX();
            int py=playerptr->getY();
            int pw=playerptr->getW();
            int ph=playerptr->getH();
            int pcx = px+0.5*pw;
            int pcy = py+0.5*ph;
            bool incontact = getContact(pcx, pcy, pw, ph);
            if(incontact){
                // std::cout<<"Trouble\n";
                playerptr->failGame();
            }
            // else{
            //     std::cout<<"Safe\n";
            // }
        }
        virtual void nextStep(double dt){
            wipe();
            // std::cout<<sqrt(vx*vx+vy*vy)<<"\n";
            x+=vx*dt;y+=vy*dt;
            vx += 0.1; vy += 0.1;
            if(randomizeMotion){
                int px=playerptr->getX();
                int py=playerptr->getY();
                int theta = rand()%180- 90 + 180*atan2(1.0*(py-y),(px-x)*1.0)/PI;
                double c = cos(PI*theta/180), s = sin(PI*theta/180);
                double ux,uy;
                // ux = vx*c + vy*s;
                // uy = vx*c - vy*s;
                ux = sqrt(vx*vx+vy*vy)*c;
                uy = sqrt(vx*vx+vy*vy)*s;
                vx = ux; vy = uy;
            }
            else{
                vx+=ax*dt;vy+=ay*dt;
            }
            if(x+w>=WINX-MARGX){vx=-abs(vx);}
            if(x<MARGX){vx=abs(vx);}
            if(y+h>=WINY-MARGY){vy=-abs(vy);}
            if(y<MARGY){vy=abs(vy);}            
            resize(x,y,w,h);
            newDraw();
            checkPlayerDist();
        };

};
class Edible: public MyEllipse{
    PlayerGon *playerptr;
    public:
        Edible(int x, int y, int w, int h):MyEllipse(x,y,w,h){
            mycolor=FL_BLUE;
        }
            void setPlayerPtr(PlayerGon *ptr){
            playerptr = ptr;
        }
        void getEaten(){
            playerptr->increment(1000);
            int tempx, tempy;
            tempx = rand()%(WINX-2*MARGX) +MARGX;
            tempy = rand()%(WINY-2*MARGY) + MARGY;
            resetPosition(tempx, tempy);
        }
        bool getContact(int pcx, int pcy, int pw, int ph){
            
            int mcx, mcy;
            mcx = x+0.5*w; mcy = y+0.5*h;
            bool incontact = false;
            if((pcx-0.5*pw<x+w&&pcx-0.5*pw>x)||(pcx+0.5*pw<x+w&&pcx+0.5*pw>x)){
                if((pcy-0.5*ph<y+h&&pcy-0.5*ph>y)||(pcy+0.5*ph<y+h&&pcy+0.5*ph>y)){
                    incontact = true;
                }
            }
            if(!incontact){
                if(sqrt((mcx-pcx)*(mcx-pcx)+(mcy-pcy)*(mcy-pcy))<0.5*(pw+w)){
                    incontact=true;
                }
            }
            return incontact;
        }
        void checkPlayerDist(){
            int px=playerptr->getX();
            int py=playerptr->getY();
            int pw=playerptr->getW();
            int ph=playerptr->getH();
            int pcx = px+0.5*pw;
            int pcy = py+0.5*ph;
            bool incontact = getContact(pcx, pcy, pw, ph);
            if(incontact){
                getEaten();
            }
            // else{
            //     std::cout<<"Safe\n";
            // }
        }
        virtual void nextStep(double dt){
            MyEllipse::nextStep(dt);
            checkPlayerDist();
        }

};
#include<string>
#include<string.h>
char *int2charstar (int v) {
 char *s = new char[sizeof(int)];
 sprintf(s,"%d",v);
 return s;
};
class MyButton : public Fl_Button {
	
	int state;
	int counter=0;
	int index[2];
	int labval;
	static int empty[2];
	public:
		
		MyButton (int x, int y, int w, int h, int i, int j); 
		int handle(int e); // e is incoming mouse event of different kinds
		int getlabval();
		void setlabval(int e);
		int getEmpty0(){return empty[0];}
		int getEmpty1(){return empty[1];}
		void setEmpty(int e0, int e1){
			empty[0]=e0;
			empty[1]=e1;
		}
 };

class ShufflerButton:public Fl_Button{
    bool solved;
    public:
    ShufflerButton(int x, int y, int w, int h, const char * c);
    int handle(int e);
    void setSolved(bool state);
    void shuffle(int order);
};
ShufflerButton::ShufflerButton(int x, int y, int w, int h, const char * c):Fl_Button(x,y,w,h,c){
solved=true;
}
class OrderButton:public Fl_Button{
	int myorder;
	public:
	static int order;
	OrderButton(int x, int y, int w, int h, const char *c, int _order);
	int handle(int e);
};
OrderButton::OrderButton(int x, int y, int w, int h, const char *c, int _order):Fl_Button(x,y,w,h,c){
myorder = _order;
}

class ImageButton:public Fl_Button{
	public:
	static std::string imagesrc;
	std::string srcfolder;
	
	ImageButton(int x, int y, int w, int h, const char* imagename);
	int handle(int e);
};
ImageButton::ImageButton(int x, int y, int w, int h, const char* imagename):Fl_Button(x, y, w, h, imagename){
	srcfolder = imagename;
}

class SolveButton:public Fl_Button{
	public:
		SolveButton(int x, int y, int w, int h, const char *l);
		void resetPuzzle();
		int handle(int e);
};
SolveButton::SolveButton(int x, int y, int w, int h, const char *l):Fl_Button(x, y, w, h, l){
	color(FL_DARK_CYAN);
	redraw();
}
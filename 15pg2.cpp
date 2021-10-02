//Image puzzle program written by Hardik Rajpal for CS154 lab4.
//Improvemenst: Shuffle algorithm made more random, images used instead of labels.
//Added more functionality of resetting the puzzle.
//For a program in which the buttons don't have images, only labels, please
//refer to the file 15gamepuzzle.cpp.
//Note, for the images to show up, you have to download the rsrc folder and keep 
//it in the same directory as the compiled cpp file.

//!!!!!!!!!!!!!!!!For compiling this program in g++, use the command:
//g++ 15pg2.cpp -o 15pg2 -lfltk -lfltk_jpeg -lfltk_images -lfltk_z
//Or remember to include the image libraries otherwise.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include <FL/Fl.H> // needed in every fltk program
#include <FL/Fl_Window.H> // needed to use the Fl_Window class
#include <FL/Fl_Button.H> // needed to use the Fl_Box class
#include<FL/Fl_Box.H>
#include "mybutton.h"
#include<string>
#include<string.h>
#include<FL/Fl_JPEG_Image.H>
using namespace std;


int MyButton::empty[2]={3,3};
int OrderButton::order=10;
string ImageButton::imagesrc = "rsrc/Numbers/image_part_0";

ImageButton *imgbuttons[5];
ShufflerButton *shuffler;
MyButton *buttons[4][4];
OrderButton *orders[3];
Fl_JPEG_Image *pieces[16];


bool issolved(){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(buttons[i][j]->getlabval()!=j+4*i+1){
				return false;
			}
		}
	}
	return true;
}

//returns inversions of the button at index wrt vals.
int getInversions(int index, int *vals, int l){
	int counter=0;
	int myval = vals[index];
	for(int i=0;i<l;i++){
		if(i<index){
			if(vals[i]>myval){
				counter++;
			}
		}
		else if(i>index){
			if(vals[i]<myval){
				counter++;
			}
		}
	}
	return counter;
}

void setEmpty(){
	for(int i=0;i<16;i++){
		if(16==buttons[i/4][i%4]->getlabval()){
			buttons[i/4][i%4]->setEmpty(i%4, i/4);
		}
	}
}

bool issolvable(){
	int n=16;
	int totalinversions=0;
	int labvals[100];
	for(int i=0;i<n;i++){
		labvals[i]=buttons[i/4][i%4]->getlabval();
	}
	for(int i=0;i<n;i++){
		totalinversions+=getInversions(i, labvals, n);
	}
	totalinversions /= 2;
	if(totalinversions%2==0){
		if (buttons[0][0]->getEmpty0() % 2 != 0) {
			return true;
		}
	}
	else{
		if (buttons[0][0]->getEmpty0() % 2 == 0) {
			return true;
		}
	}
	return false;
}

void randomizeTiles(int n, int nswaps){
	bool swapped[100];
	for(int i=0;i<n;i++){
		swapped[i]=false;
	}
	while(nswaps>0){
		int rand1 = rand()%n;
		int rand2 = rand()%n;
		int labval1, labval2;
		if(rand1!=rand2){
			if(swapped[rand1]==false||swapped[rand2]==false){
				labval1 = buttons[rand1/4][rand1%4]->getlabval();
				labval2 = buttons[rand2/4][rand2%4]->getlabval();
				buttons[rand1/4][rand1%4]->setlabval(labval2);
				buttons[rand2/4][rand2%4]->setlabval(labval1);
				swapped[rand1]=true;
				swapped[rand2]=true;
			}
		}
		nswaps--;
	}
}


int ShufflerButton::handle(int e){
	Fl_Button::handle(e);
	if(e==FL_PUSH){
		shuffle(OrderButton::order);
	}
	
	
	return 0;
	
}
void ShufflerButton::setSolved(bool state=true){
	solved = state;
}
void ShufflerButton::shuffle(int order){
	randomizeTiles(16, order);
	while(!issolvable()){
		randomizeTiles(16, order);
	}
	setEmpty();
	label("Shuffle more?");
}
int OrderButton::handle(int e){
	Fl_Button::handle(e);
	if(e==FL_PUSH){
		if(order==myorder){
			return 0;
		}
		order = myorder;
		orders[0]->color(FL_GREEN);orders[0]->redraw();
		orders[1]->color(FL_YELLOW);orders[1]->redraw();
		orders[2]->color(FL_RED);orders[2]->redraw();
		this->color(FL_BLUE);
		redraw();
	}
	return 0;
}

MyButton :: MyButton (int x,int y, int w, int h, int j, int i) : Fl_Button (x,y,w,h) {
	state =0;
	index[0]=j;
	index[1]=i;
	setlabval(4*i+j+1);
}
int MyButton::getlabval(){
	return labval;
}
void MyButton::setlabval(int e){
	labval = e;
	image(pieces[e-1]);
	redraw();
}
int MyButton :: handle (int e) {
	Fl_Button::handle(e);
	if (e==FL_PUSH) {	
		if(empty[0]==index[0]){
			if(empty[1]==index[1]){
				return 0;
			}
			if(abs(empty[1]-index[1])>1){
				return 0;
			}
			buttons[empty[1]][empty[0]]->setlabval(labval);
			
			setlabval(16);
			
			empty[1]=index[1];
		}
		else if(empty[1]==index[1]){
			if(empty[0]==index[0]){
				return 0;
			}
			if(abs(empty[0]-index[0])>1){
				return 0;
			}
			buttons[empty[1]][empty[0]]->setlabval(labval);
			setlabval(16);
			empty[0]=index[0];
			
		}
		redraw();
		state = 1;
		if(issolved()){
			shuffler->label("Solved! Press to Shuffle again");
			shuffler->setSolved(true);
		}
		else{
			shuffler->label("Shuffle more?");
			shuffler->setSolved(false);
		}
	}
	else {
		state = 0;
	}
	return 1;
};
int SolveButton::handle(int e){
	Fl_Button::handle(e);
	if(e==FL_PUSH){
		resetPuzzle();
	}
	return 0;
}
void SolveButton::resetPuzzle(){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			buttons[i][j]->setlabval(4*i+j+1);
		}
	}
	setEmpty();
}


int ImageButton::handle(int e){
	Fl_Button::handle(e);
	if(e==FL_PUSH){
		if(imagesrc=="rsrc/"+srcfolder+"/image_part_0"){
			return 0;
		}
		imagesrc = "rsrc/"+srcfolder+"/image_part_0";
		for(int i=0;i<16;i++){
			if(i+1<10){
				//delete pieces[i];
				pieces[i]= new Fl_JPEG_Image((imagesrc+"0"+to_string(i+1)+".jpg").c_str());
			}
			else{
				//delete pieces[i];
				pieces[i]= new Fl_JPEG_Image((imagesrc+to_string(i+1)+".jpg").c_str());
			}
		}
		for(int i=0;i<16;i++){
			buttons[i/4][i%4]->setlabval(buttons[i/4][i%4]->getlabval());
			
		}

		for(int i=0;i<5;i++){
			imgbuttons[i]->color(180);
			imgbuttons[i]->redraw();
		}
		color(180+20);
		color2(FL_GREEN);
		redraw();
	}
	return 0;
}




int main(int argc, char *argv[]) {
	Fl_Window *window;
	Fl_Box *diff;//difficulty instruction box
	Fl_Box *credits;
	Fl_Box *pic;//picture choice instruction box
	SolveButton *solver;
	int xoffset_puzzle, yoffset_puzzle;//offsets of puzzle box
	int xoffset_pic, yoffset_pic, pic_dist;//offsets of the column giving choices for the puzzle pictures and the distance between those buttons.
	srand(time(0));

	
	//loading the relevant puzzle pieces below:
	for(int i=0;i<16;i++){
		if(i+1<10){
		pieces[i]= new Fl_JPEG_Image((ImageButton::imagesrc+"0"+to_string(i+1)+".jpg").c_str());
		}
		else{
		pieces[i]= new Fl_JPEG_Image((ImageButton::imagesrc+to_string(i+1)+".jpg").c_str());
		}
	}

	window = new Fl_Window (650,400,"Why's it called Puzzles?"); // outer window
	window->color(FL_WHITE);
	
	
	xoffset_puzzle=160;yoffset_puzzle=100;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			const char * s = int2charstar(4*i+j+1);
			if(i+4*j!=15){
				buttons[i][j] = new MyButton(40*j+xoffset_puzzle, 40*i+yoffset_puzzle, 40,40, j,i);
			}
			else{
				buttons[i][j] = new MyButton(40*j+xoffset_puzzle, 40*i+yoffset_puzzle, 40,40, j,i);
			}		
		}
	}

	buttons[0][0]->setEmpty(3, 3);
	
	diff = new Fl_Box(410, 130, 100, 20, "Select Difficulty: ");
	
	orders[0] = new OrderButton(400, 160, 100, 40, "Easy", 10);
	orders[0]->color(FL_BLUE);
	orders[1] = new OrderButton(400, 220, 100, 40, "Okay", 20);
	orders[1]->color(FL_YELLOW);
	orders[2] = new OrderButton(400, 280, 100, 40, "Hard", 30);
	orders[2]->color(FL_RED);
	
	shuffler = new ShufflerButton(400, 80, 220, 40, "Shuffle more?");
	shuffler->color(FL_CYAN);
	shuffler->shuffle(OrderButton::order);
	
	solver = new SolveButton(400, 20, 220, 40, "Reset Puzzle.");
	xoffset_pic = 30;yoffset_pic = 50;pic_dist=60;
	pic = new Fl_Box(30, yoffset_pic-30, 100, 20, "Select puzzle picture:");
	imgbuttons[0] = new ImageButton(30, yoffset_pic, 100, 40, "Numbers");
	imgbuttons[1] = new ImageButton(30, yoffset_pic+pic_dist, 100, 40, "Bob");
	imgbuttons[2] = new ImageButton(30, yoffset_pic+2*pic_dist, 100, 40, "Star");
	imgbuttons[3] = new ImageButton(30, yoffset_pic+3*pic_dist, 100, 40, "Swift");
	imgbuttons[4] = new ImageButton(30, yoffset_pic+4*pic_dist, 100, 40, "Paris");
	for(int i=0;i<5;i++){
		imgbuttons[i]->color(180);
	}
	imgbuttons[0]->color(180+20);
	
	credits = new Fl_Box(470, 350, 100, 50, "Puzzles by Hardik Rajpal. 2021");
	
	window->show();
	return(Fl::run());
}



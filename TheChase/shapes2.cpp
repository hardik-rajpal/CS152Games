//Code written by Hardik Rajpal for lab5 of CS154.
//The aim is to maximize the score, which increases every second in multiple related to the 
//green circle's radius.
//The red squares are obstacles that we have to avoid touching.

#include <FL/Fl.H> // needed in every fltk program
#include <FL/Fl_Window.H> // needed to use the Fl_Window class
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include<FL/fl_message.H>
#include<iostream>
#include<string>
#include<string.h>
#include<cstring>
#include "myshapes.h"
using namespace std;
ObstacleRect *obstacles[3];
PlayerGon *player;
Fl_Box *scoreboard;
Edible *eatables[3];
char *myscr;
double maindt;
double stepdt;


void nextStepsCaller(void*){
	// player->increment(player->getW()/50);
	for(int i=0;i<3;i++){
		obstacles[i]->nextStep(stepdt);
		eatables[i]->nextStep(stepdt);
	}
	// e1->nextStep(stepdt);

	// player->nextStep(stepdt);
	// setScore(scoreboard, score);
	Fl::redraw();
	// cout<<"Score: "<<score<<"\n";
	Fl::repeat_timeout(maindt, nextStepsCaller);
}
void PlayerGon::failGame(){
	// cout<<"Fail";
	eatables[0]->resetPosition(MARGX + 10, MARGY + 10);
	eatables[1]->resetPosition(WINX - MARGX - 10, MARGY + 10);
	eatables[2]->resetPosition(MARGX + 10, WINY - MARGY - 10);
	for(int i=0;i<3;i++){
		
		obstacles[i]->resetPosition(MARGX+5+100*i, MARGY+5);
	}
	player->resetPosition(WINX-MARGX-5, WINY-MARGY-5);
	fl_alert("Game Failed! Restarting!\n Your score: %i", score);
	score=0;
	scoreboard->label("Score: 0");
	Fl::redraw();
	return;
}
int main(int argc, char *argv[]) {
// cout<<"Hi";
	// score=0;
	// strcpy(myscr, "Hi");
	maindt = 0.1;
	stepdt = 1;
	Fl_Window *window;
	window = new Fl_Window (WINX,WINY,"The Chase"); // outer window
	scoreboard = new Fl_Box(0, 0, WINX, 30, "Score: 0");
	player = new PlayerGon(700, 700, 50, 50);	
	eatables[0] = new Edible(MARGX+10, MARGY+10, 20, 20);
	eatables[1] = new Edible(WINX-MARGX-10, MARGY+10, 20, 20);
	eatables[2] = new Edible(MARGX+10, WINY-MARGY-10, 20, 20);
	for (int i=0; i<3; i++) {
            obstacles[i] = new ObstacleRect(10+i*100,40+i*100,OBSW,OBSH);
			obstacles[i]->setMotionVariables(15, 15, 0, 0, true);
			obstacles[i]->setPlayerPtr(player);
			
			eatables[i]->setPlayerPtr(player);
	    //    new MyCircle(300,300+i*100, 35);
	}
	player->setScoreBoard(scoreboard);
	
	
	window->color(FL_WHITE);
    window->end();
    window->show();
	fl_alert("Welcome to The Chase!\nInstructions:\nMove the green circle by hovering your cursor over it.\nIncrease your points by moving around and collecting the blue pills.\nUse the up and down keys to change the size of the green circle.\nA larger circle collects more points by moving than a smaller one.\nAvoid the red squares because um, Game Over if you don't.");
	Fl::add_timeout(1, nextStepsCaller);
	return(Fl::run());  // the process waits from here on for events
}

#pragma once
#ifndef READ SOCKET_H_INCLUDED
#define READ SOCKET_H_INCLUDED

#include <string>
#include "json\json.h"
#include <iostream>
#include "stdafx.h"
using namespace std;

class outcontent {
public:
	string x;
	string y;
	string z;
};

class readsocket_read {
public:
	string outcommand;
	string outreadDetail;
};

class readsocket_move {
public:
	string outcommand;
	string outmode;
	string outvelcityMode;
	string outpathMode;
	string outmaxVel;
	string outmaxAcc;
	outcontent outtarget;
};

string getcommand(string recv);
string command_connect(string recv);
readsocket_read command_read(string recv);
readsocket_move command_move(string recv);


#endif
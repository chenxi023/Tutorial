#include "string.h"
#include "json\json.h"
#include <iostream>
#include "read socket.h"
#include "stdafx.h"
#include "json\value.h"
#include "json\reader.h"


using namespace std;

string getcommand(string recv) {

	std::string strValue = recv;

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strValue, value))
	{
		std::string outcommand = value["command"].asString();
		//std::cout << "command"<< outcommand << std::endl;
		if (outcommand == "") {
			outcommand = "faild";
		}
		return outcommand;
	}
}

string command_connect(string recv) {
	using namespace std;
	std::string strValue = recv;

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strValue, value))
	{
		std::string outcommand = value["command"].asString();
		std::cout << "command" << outcommand << std::endl;

		return outcommand;	
	}
}

readsocket_read command_read(string recv) {
	using namespace std;
	std::string strValue = recv;

	Json::Reader reader;
	Json::Value value;

	class readsocket_read readvalue;

	if (reader.parse(strValue, value))
	{
		readvalue.outcommand = value["command"].asString();
		readvalue.outreadDetail = value["readDetail"].asString();
		//std::cout << readvalue.outcommand << std::endl;
		std::cout << "command" << readvalue.outcommand << std::endl;
		std::cout <<"readDetail" <<readvalue.outreadDetail << std::endl;

		return readvalue;
	}
}


readsocket_move command_move(string recv) {
	using namespace std;
	std::string strValue = recv;

	Json::Reader reader;
	Json::Value value;

	class readsocket_move readvalue;

	if (reader.parse(strValue, value))
	{
		    readvalue.outcommand = value["command"].asString();
			readvalue.outmode = value["moveMode"].asString();
			std::cout << "command" << readvalue.outcommand << std::endl;
			std::cout << "moveMode"<<readvalue.outmode << std::endl;
		
		if (readvalue.outmode == "default") {
			Json::Value &target= value["target"];
			readvalue.outtarget.x = target["x"].toStyledString();
			readvalue.outtarget.y = target["y"].toStyledString();
			readvalue.outtarget.z = target["z"].toStyledString();
			
			//Json::Value &js = target["x"];
			
			std::cout << "target" << "x" << readvalue.outtarget.x << std::endl;
			std::cout << "target" << "y" << readvalue.outtarget.y << std::endl;
			std::cout << "target" << "z" << readvalue.outtarget.z << std::endl;
		}
		if (readvalue.outmode == "custom") {
			readvalue.outvelcityMode = value["velcityMode"].asString();
			string pathmode = value["pathMode"].asString();
			if (pathmode=="cricle") {
				readvalue.outpathMode = 1;
			}
			else if (pathmode == "line") {
				readvalue.outpathMode = 2;
				}
			else if (pathmode == "cricle_line") {
				readvalue.outpathMode = 3;
			}
			readvalue.outmaxVel = value["maxVel"].asString();
			readvalue.outmaxAcc = value["maxAcc"].asString();
			Json::Value &target = value["target"];
			readvalue.outtarget.x = target["x"].toStyledString();
			readvalue.outtarget.y = target["y"].toStyledString();
			readvalue.outtarget.z = target["z"].toStyledString();
			std::cout << "velcityMode" << readvalue.outvelcityMode << std::endl;
			std::cout << "pathMode" << readvalue.outpathMode << std::endl;
			std::cout << "maxVel" << readvalue.outmaxVel << std::endl;
			std::cout << "maxAcc" << readvalue.outmaxAcc << std::endl;
			std::cout << "target" << "x" << readvalue.outtarget.x << std::endl;
			std::cout << "target" << "y" << readvalue.outtarget.y << std::endl;
			std::cout << "target" << "z" << readvalue.outtarget.z << std::endl;
		}
		return readvalue;


	}
}

/*
readsocket command_read(string recv) {
	using namespace std;
	std::string strValue = recv;

	Json::Reader reader;
	Json::Value value;

	class readsocket readvalue;

	if (reader.parse(strValue, value))
	{
		readvalue.out = value["command"].asString();
		//std::cout << readvalue.out << std::endl;
		if (readvalue.out == "read") {
			readvalue.outdetail = value["readDetail"].asString();
			//std::cout << readvalue.outdetail << std::endl;
		}
		if (readvalue.out == "move") {
			readvalue.outdetail = value["moveMode"].asString();
			//std::cout << readvalue.outdetail << std::endl;
		}
		return readvalue;


	}
}
*/
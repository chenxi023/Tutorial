// OPCClientDemo.cpp : Defines the entry point for the console application.
//

/*
OPCClientToolKit
Copyright (C) 2005 Mark C. Beharrell

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/

#include <stdio.h>
#include "opcda.h"
#include "OPCClient.h"
#include "OPCHost.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include "OPCItem.h"
#include <sys\timeb.h>
#include "stdafx.h"
//#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#include "json\json.h"
#include <string>
#include <iostream>
#include "massage.h"
//#include "ws2def.h"
#include "timestamp_to_standard.h"
#include "read socket.h"



/**
* Code demonstrates
* 1) Browsing local servers.
* 2) Connection to local server.
* 3) Browsing local server namespace
* 4) Creation of OPC item and group
* 5) synch and asynch read of OPC item.
* 6) synch and asynch write of OPC item.
* 7) The receipt of changes to items within a group (subscribe)
* 8) group refresh.
* 9) Synch read of multiple OPC items.
*/


/**
* Handle asynch data coming from changes in the OPC group
*/
class CMyCallback:public IAsynchDataCallback{
	public:
		void OnDataChange(COPCGroup & group, CAtlMap<COPCItem *, OPCItemData *> & changes){
			std::printf("Group %s, item changes\n", group.getName());
			POSITION pos = changes.GetStartPosition();
			while (pos != NULL){
				COPCItem * item = changes.GetNextKey(pos);
				std::printf("-----> %s\n", item->getName());
			}
		}
		
};



/**
*	Handle completion of asynch operation
*/
class CTransComplete:public ITransactionComplete{
	CString completionMessage;
public:
	CTransComplete(){
		completionMessage = "Asynch operation is completed";
	};

	void complete(CTransaction &transaction){
		std::printf("%s\n",completionMessage);
	}

	void setCompletionMessage(const CString & message){
		completionMessage = message;
	}
};


//---------------------------------------------------------
// main


#define MESSAGEPUMPUNTIL(x)	while(!x){{MSG msg;while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE)){TranslateMessage(&msg);DispatchMessage(&msg);}Sleep(1);}}

using namespace std;

int _tmain(int argc, char* argv[])
{
	/*int m_stampTime1 = 1285308732;
	stamp_to_standard(m_stampTime1);

	int m_stampTime2 = 1286899460;
	stamp_to_standard(m_stampTime2);
	*/

	/****************************OPC 配置**************************/
	COPCClient::init();

	//CString hostName = "pcitco89";
	CString hostName = "192.168.1.102";
	COPCHost *host = COPCClient::makeHost(hostName);

	//List servers
	CAtlArray<CString> localServerList;
	host->getListOfDAServers(IID_CATID_OPCDAServer20, localServerList);
	unsigned i = 0;
	for (; i < localServerList.GetCount(); i++) {
		std::printf("%s\n", localServerList[i]);
	}

	// connect to OPC
	CString serverName = "CoDeSys.OPC.DA";//"CAEN.HVOPCServer.1";
	COPCServer *opcServer = host->connectDAServer(serverName);

	// Check status
	ServerStatus status;
	opcServer->getStatus(status);
	std::printf("Server state is %ld\n", status.dwServerState);

	// browse server
	CAtlArray<CString> opcItemNames;
	opcServer->getItemNames(opcItemNames);
	std::printf("Got %d names\n", opcItemNames.GetCount());
	for (i = 0; i < opcItemNames.GetCount(); i++) {
		std::printf("%s\n", opcItemNames[i]);
	}

	// make group
	unsigned long refreshRate;
	COPCGroup *group = opcServer->makeGroup("Group", true, 1000, refreshRate, 0.0);
	CAtlArray<COPCItem *> items;

	/**********make item**********
	CString changeChanNameName = "PLC_GW3.Application.PLC_PRG.j";
	COPCItem * readWritableItem = group->addItem(changeChanNameName, true);
	*/

	// make several items
	CAtlArray<CString> itemNames;
	CAtlArray<COPCItem *>itemsCreated;
	CAtlArray<HRESULT> errors;

	itemNames.Add("PLC_GW3.Application.IoConfig_Globals.DeltaMotor1.fActPosition");//0
	itemNames.Add("PLC_GW3.Application.IoConfig_Globals.DeltaMotor1.fActTorque");//1
	itemNames.Add("PLC_GW3.Application.IoConfig_Globals.DeltaMotor1.fActVelocity");//2
	itemNames.Add("PLC_GW3.Application.IoConfig_Globals.DeltaMotor2.fActPosition");//3
	itemNames.Add("PLC_GW3.Application.IoConfig_Globals.DeltaMotor2.fActTorque");//4
	itemNames.Add("PLC_GW3.Application.IoConfig_Globals.DeltaMotor2.fActVelocity");//5
	itemNames.Add("PLC_GW3.Application.IoConfig_Globals.DeltaMotor3.fActPosition");//6
	itemNames.Add("PLC_GW3.Application.IoConfig_Globals.DeltaMotor3.fActTorque");//7
	itemNames.Add("PLC_GW3.Application.IoConfig_Globals.DeltaMotor3.fActVelocity");//8
	itemNames.Add("PLC_GW3.Application.tjDelta.Target_osc_X");//9
	itemNames.Add("PLC_GW3.Application.tjDelta.Target_osc_Y");//10
	itemNames.Add("PLC_GW3.Application.tjDelta.Target_osc_Z");//11
	itemNames.Add("PLC_GW3.Application.Global_list.maxAcc");//12
	itemNames.Add("PLC_GW3.Application.Global_list.maxVel");//13
	itemNames.Add("PLC_GW3.Application.tjDelta.Path_mode");//14
	itemNames.Add("PLC_GW3.Application.tjDelta.Velocity_mode");//15
	itemNames.Add("PLC_GW3.Application.tjDelta.move_temp");//16
	itemNames.Add("PLC_GW3.Application.tjDelta.Path_osc");//17
	itemNames.Add("PLC_GW3.Application.tjDelta.bwork_osc");//18

	if (group->addItems(itemNames, itemsCreated, errors, true) != 0) {
		std::printf("Item create failed\n");
		//else 
		//	cout<<"Item create ok\n";
	}
	/*********************************SOCKET SERVER 配置**************************************************/
	const int BUF_SIZE = 1000;
	const int BUF_SEND_SIZE = 1000;

	WSADATA wsd; //WSADATA变量
	SOCKET sServer; //服务器套接字
	SOCKET sClient; //客户端套接字
	SOCKADDR_IN addrServ;; //服务器地址
	char buf[BUF_SIZE]; //接收数据缓冲区
	char sendBuf[BUF_SEND_SIZE];//返回给客户端得数据
	int retVal; //返回值

				//初始化套结字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		std::cout << "WSAStartup failed!" << endl;
		return 1;
	}

	//创建套接字
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		std::cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源;
		return -1;
	}

	//服务器套接字地址 
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;

	//绑定套接字
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));

	if (SOCKET_ERROR == retVal)
	{
		std::cout << "bind failed!" << endl;
		closesocket(sServer); //关闭套接字
		WSACleanup(); //释放套接字资源;
		return -1;
	}
	while (true) {
		//开始监听 
		retVal = listen(sServer, 1);
		if (SOCKET_ERROR == retVal)
		{
			std::cout << "listen failed!" << endl;
			closesocket(sServer); //关闭套接字
			WSACleanup(); //释放套接字资源;
			return -1;
		}

		//接受客户端请求
		sockaddr_in addrClient;
		int addrClientlen = sizeof(addrClient);
		sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
		bool connectok;
		bool disconnectok = false;

		if (INVALID_SOCKET == sClient)
		{
			std::cout << "accept failed!" << endl;
			closesocket(sServer); //关闭套接字
			WSACleanup(); //释放套接字资源;
			return -1;
		}
		//else
		//{
			//connectok = true;
		//}
		/*******************************SOCKET SERVER RECV *******************************************/
		while (true) {
			//接收客户端数据
			ZeroMemory(buf, BUF_SIZE);
			retVal = recv(sClient, buf, BUF_SIZE, 0);
			if (SOCKET_ERROR == retVal)
			{
				std::cout << "recv failed!" << endl;
				closesocket(sServer); //关闭套接字
				closesocket(sClient); //关闭套接字
				WSACleanup(); //释放套接字资源;
				return -1;
			}
			//if (disconnectok)//断开连接
				//break;
			string buf_data;
			buf_data = buf;
			if (buf_data == "") {
				break;
			}
			std::cout << "客户端发送的数据: " << buf_data << endl;
			std::cout << "客户端发送的数据: " << getcommand(buf) << endl;
			int command_case;
			if (getcommand(buf) == "connect") {
				command_case = 0;
			}
			else if (getcommand(buf) == "read") {
				command_case = 1;
			}
			else if (getcommand(buf) == "move") {
				command_case = 2;
				
			}
			else if (getcommand(buf) == "disconnect") {
				command_case = 3;
			}
			else {
				command_case = 4;
			}

			int j;
			float PLC_Data[100];
			memset(sendBuf, 0, 1000);
			string command = command_connect(buf);
			readsocket_read readvalue_read = command_read(buf);
			readsocket_move readvalue_move = command_move(buf);
			OPCItemData data;

		switch (command_case) {

			case 0:
				//string command = command_connect(buf);
				
				data.vDataValue.vt = VT_BOOL;
				data.vDataValue.boolVal = true;
				itemsCreated[18]->writeSync(data.vDataValue);

				itemsCreated[18]->readSync(data, OPC_DS_DEVICE);
				connectok = data.vDataValue.boolVal;
				if (connectok) {
					strcpy_s(sendBuf, answertok().c_str());
					std::cout << "向客户端发送数据: ";
					std::cout << answertok() << std::endl;
				}
				else
				{
					strcpy_s(sendBuf, answerfaild().c_str());
					std::cout << "向客户端发送数据: ";
					std::cout << answerfaild() << std::endl;
				}
				break;


			case 1:
				//string command = command_connect(buf);
				//readsocket_read readvalue = command_read(buf);
				if (readvalue_read.outreadDetail == "motors") {
					for (j = 0; j < 9; j++) {
						COPCItem * m_readWritable = itemsCreated[j];
						CAtlArray<CPropertyDescription> propDesc;
						m_readWritable->getSupportedProperties(propDesc);
						std::printf("Supported properties for %s\n", m_readWritable->getName());

						OPCItemData data;
						m_readWritable->readSync(data, OPC_DS_DEVICE);
						PLC_Data[j] = data.vDataValue.dblVal;
						//std::printf("Synch read quality %d value %s timestamp %d\n", data.wQuality, data.vDataValue.dblVal, data.ftTimeStamp);
						std::cout << "Synch read quality" << data.wQuality << "value" << data.vDataValue.dblVal << std::endl;

					}
					double motor1_position = PLC_Data[0];
					double motor1_torque = PLC_Data[1];
					double motor1_velocity = PLC_Data[2];
					double motor2_position = PLC_Data[3];
					double motor2_torque = PLC_Data[4];
					double motor2_velocity = PLC_Data[5];
					double motor3_position = PLC_Data[6];
					double motor3_torque = PLC_Data[7];
					double motor3_velocity = PLC_Data[8];
					strcpy_s(sendBuf, motors(motor1_position, motor1_velocity, motor1_torque,
						motor2_position, motor2_velocity, motor2_torque,
						motor3_position, motor3_velocity, motor3_torque).c_str());//字符串到字符数组
					std::cout << "向客户端发送数据: ";
					std::cout << motors(motor1_position, motor1_velocity, motor1_torque,
						motor2_position, motor2_velocity, motor2_torque,
						motor3_position, motor3_velocity, motor3_torque) << std::endl;
				}
				else
				{
					strcpy_s(sendBuf, answerfaild().c_str());
					std::cout << "向客户端发送数据: ";
					std::cout << answerfaild() << std::endl;
				}
				break;

			case 2:
				//string command = command_connect(buf);
				//readsocket_move readvalue = command_move(buf);
				if (readvalue_move.outmode == "default") {
					//OPCItemData data;
					data.vDataValue.vt = VT_R8;
					data.vDataValue.dblVal = atof(readvalue_move.outtarget.x.c_str());
					itemsCreated[9]->writeSync(data.vDataValue);
					data.vDataValue.dblVal = atof(readvalue_move.outtarget.y.c_str());
					itemsCreated[10]->writeSync(data.vDataValue);
					data.vDataValue.dblVal = atof(readvalue_move.outtarget.z.c_str());
					itemsCreated[11]->writeSync(data.vDataValue);
					//data.vDataValue.vt = VT_BOOL;
					//data.vDataValue.boolVal = true;
					//itemsCreated[18]->writeSync(data.vDataValue);
					data.vDataValue.vt = VT_UI1;
					data.vDataValue.intVal = 1;
					itemsCreated[14]->writeSync(data.vDataValue);
					data.vDataValue.vt = VT_BOOL;
					data.vDataValue.boolVal = true;
					itemsCreated[16]->writeSync(data.vDataValue);
					bool movedefultok=true;
					bool movefaildfalg = true;
					while (movedefultok) {
						itemsCreated[16]->readSync(data, OPC_DS_DEVICE);
						//printf("Synch read quality %d value %d\n", data.wQuality, data.vDataValue.boolVal);
						movedefultok = data.vDataValue.boolVal;
					}
					//bool movedefultok = true;//moveok是否从PLC中获得，目标点走成功？
					itemsCreated[17]->readSync(data, OPC_DS_DEVICE);
					movefaildfalg = data.vDataValue.boolVal;
					if (((movedefultok)&(movefaildfalg)) ==false) {
						strcpy_s(sendBuf, answertok().c_str());
						std::cout << "向客户端发送数据: ";
						std::cout << answertok() << std::endl;
						
					}
					else
					{
						strcpy_s(sendBuf, answerfaild().c_str());
						std::cout << "向客户端发送数据: ";
						std::cout << answerfaild() << std::endl;
					}
				}
				else if (readvalue_move.outmode == "custom") {
					//OPCItemData data;
					data.vDataValue.vt = VT_R8;
					data.vDataValue.dblVal = atof(readvalue_move.outtarget.x.c_str());
					itemsCreated[9]->writeSync(data.vDataValue);
					data.vDataValue.dblVal = atof(readvalue_move.outtarget.y.c_str());
					itemsCreated[10]->writeSync(data.vDataValue);
					data.vDataValue.dblVal = atof(readvalue_move.outtarget.z.c_str());
					itemsCreated[11]->writeSync(data.vDataValue);
					data.vDataValue.dblVal = atof(readvalue_move.outmaxVel.c_str());
					itemsCreated[12]->writeSync(data.vDataValue);
					data.vDataValue.dblVal = atof(readvalue_move.outmaxAcc.c_str());
					itemsCreated[13]->writeSync(data.vDataValue);

					data.vDataValue.vt = VT_UI1;
					data.vDataValue.intVal = atof(readvalue_move.outpathMode.c_str());
					itemsCreated[14]->writeSync(data.vDataValue);
					data.vDataValue.vt = VT_BOOL;
					data.vDataValue.boolVal = true;
					itemsCreated[16]->writeSync(data.vDataValue);
					bool movedcustom = true;
					bool movefaildfalg2 = true;
					while (movedcustom) {
						itemsCreated[16]->readSync(data, OPC_DS_DEVICE);
						//printf("Synch read quality %d value %d\n", data.wQuality, data.vDataValue.boolVal);
						movedcustom = data.vDataValue.boolVal;
					}
					//bool movedcustom = true;//moveok是否从PLC中获得，目标点走成功？
					itemsCreated[17]->readSync(data, OPC_DS_DEVICE);
					movefaildfalg2 = data.vDataValue.boolVal;
					if (((movedcustom)&(movefaildfalg2)) == false) {
					//if (movedcustom==false) {
						strcpy_s(sendBuf, answertok().c_str());
						std::cout << "向客户端发送数据: ";
						std::cout << answertok() << std::endl;
					}
					else
					{
						strcpy_s(sendBuf, answerfaild().c_str());
						std::cout << "向客户端发送数据: ";
						std::cout << answerfaild() << std::endl;
					}
				}
				else{
					strcpy_s(sendBuf, answerfaild().c_str());
					std::cout << "向客户端发送数据: ";
					std::cout << answerfaild() << std::endl;
				}
				break;
			case 3:
				data.vDataValue.vt = VT_BOOL;
				data.vDataValue.boolVal = false;
				itemsCreated[18]->writeSync(data.vDataValue);

				itemsCreated[18]->readSync(data, OPC_DS_DEVICE);
				disconnectok = data.vDataValue.boolVal;
				if (disconnectok==false) {
					strcpy_s(sendBuf, answertok().c_str());
					std::cout << "向客户端发送数据: ";
					std::cout << answertok() << std::endl;
				}
				else
				{
					strcpy_s(sendBuf, answerfaild().c_str());
					std::cout << "向客户端发送数据: ";
					std::cout << answerfaild() << std::endl;
				}
				break;

			case 4:
				strcpy_s(sendBuf, answerfaild().c_str());
				std::cout << "向客户端发送数据: ";
				std::cout << answerfaild() << std::endl;
				break;
			}



			send(sClient, sendBuf, strlen(sendBuf), 0);
		}

		/**********************************SOCKET SERVER CLOSE************************************/
			//退出

		closesocket(sClient); //关闭套接字
		
	}

  closesocket(sServer); //关闭套接字
  WSACleanup(); //释放套接字资源;
  return 0;
}
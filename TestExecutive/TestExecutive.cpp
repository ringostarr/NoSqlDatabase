/////////////////////////////////////////////////////////////////////
// NoSqlDb.h - key/value pair in-memory database                   //
//  Akshay,
//  708,S beech Street #1 ,Syracuse .
//  Akshay@syr.edu
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   CSE687 Pr1, Spring 2017
//	Reference: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017    //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Demostrate Requirements

Public Interface:
=================
		Persisttoxml() : persist string to xml
		req5() : requirement 5
		req4() : requirement 4
		req3() : part of requirement 3 : addition of key/value
		req3del: part of requirement 3 : deleteion of key/value
		req7() : requirement 7
		req8() :  Requirement 8
		req9() :requirement 9
		req2() : requirement 2
		req12(): bonus requirement
Build Process:
==============
Required files
- NoSqlDb.h,QueryEngine.h
Build commands (either one)
- devenv Project1HelpS06.sln
- cl /EHsc /test_nosqldb NoSqlDb.cpp /link setargv.obj

Maintenance History:
====================
ver1.0 : Initial Build

*/
#define _CRT_SECURE_NO_WARNINGS
#include "../NoSqlDb/NoSqlDb.h"
#include "../NoSqlDb/QueryEngine.h"
#include <iostream>
#include<ctime>
#include<iomanip>
#include<chrono>
#include<thread>
#include<future>
#include<functional>
using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
template<typename T>
bool persisttoxml(NoSqlDb<T>db, int y,std::string xmlpath)
{
	DBWrapper<T> wrap(db);
 wrap.save(xmlpath);
	std::this_thread::sleep_for(std::chrono::seconds(y));
	std::cout << "Writing to file with name :"<<xmlpath<<"\n";
	return true;
	
}
template<typename T>
NoSqlDb<T> req5(NoSqlDb<T> restoreddb)
{
	DBWrapper<std::string> wrapper;
	std::cout << "====================Requirement 5 , Restoring Database from file test.xml===================\n";
	restoreddb = wrapper.restore("test.xml");
	Keys ks = restoreddb.keys();
	for (Key k : ks)
	{
		std::cout << "\n Key:" << k;
		std::cout << restoreddb.value(k).show();
	}
	std::cout << "=======================================================================================\n";
	return restoreddb;
}
template<typename T>
NoSqlDb<T> req4(NoSqlDb<T> restoreddb)
{
	std::cout << "================Requirement4 ,Editing Data in above database=======================================\n";
	Element<std::string> ele;
	ele.name = "Utilities";
	ele.category = "Utility Library";
	ele.data = "Edit:Provide Utilty such as putline function to other packages";
	ele.description = "Provides utility functions for Convert Package";
	//ele.childrel = { "Contains nothing this is an edit" };
	time_t now = time(NULL);
	struct tm* timex = std::localtime(&now);
	char buffer[32];
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
	ele.timeDate = buffer;
	restoreddb.edit("Utilities", ele);
	std::cout << "\n Editing Utilities \n\n";
	Keys ks = restoreddb.keys();
	for (Key k : ks)
	{
		std::cout << "\n Key:" << k;
		std::cout << restoreddb.value(k).show();
	}
	std::cout << "========================================================================================\n";
	return restoreddb;
}
template<typename T>
NoSqlDb<T> req7(NoSqlDb<T> restoreddb)
{
	std::cout << "===============Requirement 7,Queries======================================================\n";
	std::cout << " =====================Query1 , The value of key==============================\n";
	QueryEngine<std::string> QE;
	std::cout << "Querying for key : XmlDocument";
	NoSqlDb<std::string> q1 = QE.speckey(restoreddb, "NoSqlDb");
	std::cout << "\n====================================================================\n";
	std::cout << "===================Query2 , The children of key============================\n";
	NoSqlDb<std::string> q2 = QE.childkey(restoreddb, "XmlDocument");
	std::cout << "\n=============================================================================\n";
	std::cout << "========Query3,Pattern in key";
	std::string pattern = "U[A-Z]*[a-z]*";
	std::cout << "Pattern is :" << pattern << "\n";
	NoSqlDb<std::string> q3 = QE.regExstr(restoreddb, "key", pattern);
	std::cout << "\n=============================================\n";
	std::cout << "\n=====Query4,specified string in itemname================\n";
	std::cout << "string = Doc\n";
	NoSqlDb<std::string> q4 = QE.itemstr(restoreddb, "Doc");
	std::cout << "\n=============================================\n";
	std::cout << "============Query5,specified string in category name================\n";
	std::cout << "string = Utility";
	NoSqlDb<std::string> q5 = QE.catstr(restoreddb, "Utility");
	std::cout << "\n=================================================================\n";
	std::cout << "\n ==================Query6,specified string in data section=================\n";
	std::cout << "string = Xml\n";
	NoSqlDb<std::string> q6 = QE.datastr(restoreddb, "Xml");
	std::cout << "\n==========================Specified Time interval===============================\n";
	std::string tim = "05:02:2017 08:50:20";
	NoSqlDb<std::string> q7 = QE.timestr(restoreddb, tim);
	return restoreddb;
}
template<typename T>
NoSqlDb<T> req8(NoSqlDb<T> restoreddb)
{
	QueryEngine<std::string> QE;
	NoSqlDb<std::string> q5 = QE.catstr(restoreddb, "Utility");
	std::cout << "\n==============Requirement 8 , Query on Query===================\n";
	std::cout << "\n =============Running Query1 with string=Convert on Query5===============\n";
	QE.speckey(q5, "Convert");
	std::cout << "\n===================================================================\n";
	return restoreddb;
}
template<typename T>
NoSqlDb<T> req9(NoSqlDb<T> restoreddb)
{
	QueryEngine<std::string> QE;
	std::string pattern = "U[A-Z]*[a-z]*";
	NoSqlDb<std::string> q1 = QE.speckey(restoreddb, "NoSqlDb");
	NoSqlDb<std::string> q5 = QE.catstr(restoreddb, "Utility");
	NoSqlDb<std::string> q3 = QE.regExstr(restoreddb, "key", pattern);
	std::cout << "\n============Requirement 9,union of keys=================\n";
	std::cout << "Running union of q2 and q3\n";
	QE.keyunion(q1, q3);
	std::cout << "\n=================================================\n";
	return restoreddb;
}
template<typename T>
NoSqlDb<T> req3(NoSqlDb<T> db,int y)
{
	Element<T> e;
	int k = y;
	std::cout << "\n===========Demonstrating part of requirement 3:Addition of key value pair=====================\n";
	e.name = "Elem" + std::to_string(k);
	e.category = "Elem" + std::to_string(k);
	e.description = "descrip" + std::to_string(k);
	e.childrel = { "Elem0" };
	e.data = "Data" + std::to_string(k);
	time_t now = time(NULL);
	struct tm* timex = std::localtime(&now);
	char buffer[32];
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
	e.timeDate = buffer;
	db.save(e.name, e);
	std::cout << "\nSaving in Database Object with Key :" << e.name << "\n";
	std::cout << "\n==================================================================================================\n";
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	return db;
}
template<typename T>
NoSqlDb<T> req3del(NoSqlDb<T>db, std::string key)
{
	std::cout << "\n=================Requirement 3, deleteing key/value pair from above db======================\n";
	std::cout << "=====deleteing element with key:" << key;
	db.del(key);
	std::cout << "\nupdated db is:\n";
	Keys ks = db.keys();
	for (Key k : ks)
	{
		std::cout << db.value(k).show();
	}
	std::cout << "\n=======================================================================\n";
	return db;
}
void req2()
{
	std::cout << "\n======Requirement2,Showing tempalte class with Widget=======\n";
	Widget wdgt;
	wdgt.wint = 1;
	wdgt.wstr = "demowidget";
	std::cout << "\n widget.wint=" << wdgt.wint << "\n";
	std::cout << "\n widget.wstr=" << wdgt.wstr << "\n";
	NoSqlDb<Widget> widgetdb;
	Element<Widget> elewdgt;
	elewdgt.name = "demo";
	//elewdgt.childrel = { "norelationships" };
	elewdgt.category = "democat";
	elewdgt.description = "demo for req2";
	time_t now = time(NULL);
	struct tm* timex = std::localtime(&now);
	char buffer[32];
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
	elewdgt.timeDate = buffer;
	
	elewdgt.data = wdgt;
	std::cout << "ENtering only one element with key:" << elewdgt.name<<"\n";
	widgetdb.save(elewdgt.name, elewdgt);
	Keys ks = widgetdb.keys();
	for (Key k : ks)
	{
		Element<Widget> e;
		e = widgetdb.value(k).ret();
		std::cout << "Name:" << e.name << "\n";
		std::cout << "Category:" << e.category << "\n";
		std::cout << "Description:"<<e.description << "\n";
		Widget w = e.data.getValue();
		std::cout << "wint:" << w.wint<<"\n";
		std::cout << "wstr:" << w.wstr << "\n";
		//std::cout << widgetdb.value(k).show();
	}
	
}
template<typename T>
NoSqlDb<T> req12(NoSqlDb<T>db, std::string key)
{
	std::cout << "\n=================Bonus requirements=======================\n";
}

int main()
{
	NoSqlDb<std::string> restoreddb1;
	NoSqlDb<std::string> restoreddb;
	restoreddb = req5(restoreddb1);
	restoreddb = req4(restoreddb);
	restoreddb = req7(restoreddb);
	restoreddb = req8(restoreddb);
	restoreddb = req9(restoreddb);
	NoSqlDb<std::string> db;
	
		bool starttime=true;
		std::cout << "\n========Requirement6,Starting AutoSave Feature==== time interval:2seconds=======================\n";
		int k = 0;
			while (starttime == true) {
				Element<std::string> e;
				
				db = req3(db, k);
				auto fool = std::async(std::launch::async, persisttoxml<std::string>, db, 2, "test2.xml");
				if (k== 2)
				{
					starttime = false;
				}
				k++;
			}
			std::cout << "====================Stopping Autosave feature=======================\n";
			req3del(db, "Elem2");
			req2();
			std::cout << "Press Any Key to Exit\n";
			std::cin.get();


}
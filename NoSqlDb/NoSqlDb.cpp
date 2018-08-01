/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include "NoSqlDb.h"
#include "QueryEngine.h"
#include <iostream>
#include<ctime>
#include<iomanip>
using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
#ifdef  test_nosqldb

 //  test_nosqldb stub

int main()
{
	

	NoSqlDb<StrData> db;
	//time_t t;
	Element<StrData> elem1;
	elem1.name = "Convert";
	elem1.category = "Utility library";
	elem1.data = "Helps Conversion to strings";
	elem1.description = "Helps Conversion to strings";
	elem1.timeDate = "";
	elem1.childrel = { "Utilities" };
	time_t now = time(NULL);
	struct tm* timex = std::localtime(&now);
	char buffer[32];
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
	elem1.timeDate = buffer;
	db.save(elem1.name, elem1);
	
	Element<StrData> elem2;
	elem2.name = "CppProperties";
	elem2.category = "Utility library";
	elem2.data = "C# properties in C++";
	elem2.description = "C# properties in C++";
	elem2.timeDate = "";
	now = time(NULL);
	timex = std::localtime(&now);
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
	elem2.timeDate = buffer;
	db.save(elem2.name, elem2);
	//melem3

	Element<StrData> elem3;
	elem3.name = "NoSqlDb";
	elem3.category = "NoSql Database";
	elem3.data = "Main package which defines NoSql db functions";
	elem3.description = "Main package which defines NoSql db functions";
	elem3.timeDate = "";
	elem3.childrel = { "Convert","XmlDocument","CppProperties" };
	now = time(NULL);
	timex = std::localtime(&now);
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
	elem3.timeDate = buffer;
	db.save(elem3.name, elem3);

	Element<StrData> elem4;
	elem4.name = "Utilities";
	elem4.category = "Utility library";
	elem4.data = "Provides utility functions for Convert Package";
	elem4.description = "Provides utility functions for Convert Package";
	elem4.timeDate = "";
	now = time(NULL);
	timex = std::localtime(&now);
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
	elem4.timeDate = buffer;
	//elem4.childrel={""}
	db.save(elem4.name, elem4);

	Element<StrData> elem5;
	elem5.name = "XmlDocument";
	elem5.category = "Xml Library";
	elem5.data = "Xml library for conversion to and from XML document";
	elem5.description = "Xml library for conversion to and from XML document";
	elem5.timeDate = "";
	now = time(NULL);
	timex = std::localtime(&now);
	elem5.childrel = { "Utilities" };
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
	elem5.timeDate = buffer;
	db.save(elem5.name, elem5);

	Element<StrData> elem6;
	elem6.name = "TestExecutive";
	elem6.category = "Executive";
	elem6.data = "Shows all the requirements of the project";
	elem6.description = "Shows all the requirements of the project";
	elem6.timeDate = "";
	elem6.childrel = { "NoSqlDb" };
	now = time(NULL);
	timex = std::localtime(&now);
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
	elem6.timeDate = buffer;
	db.save(elem6.name, elem6);

	
	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";

	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";
	/*QueryEngine<StrData> QE;
	QE.speckey(db, "elem1");
	std::cout << "\n Searching for 1 in itmes";
	QE.itemstr(db, "1");
	std::cout << "\n\nSearching keys where child is abc\n";
	QE.childkey(db, "abc");
	std::cout << "\n Data string\n";
	QE.datastr(db, "elem1's");
	QE.regExstr(db, "name","([A-Z]*[a-z])+1");
	QE.timestr(db, "ac", "asd");*/
	std::string a = "alysdfd";
	std::move(a);
	std::string docx;
	DBWrapper<std::string> doc(db);
	docx = doc.save();
	std::cout<<doc.save();
	NoSqlDb<std::string> test;
	DBWrapper<std::string> newwrap;
	NoSqlDb<std::string> temp;
	temp=newwrap.restore("test.xml");
	Keys ks= temp.keys();

	std::cout << "\n restoring from file\n\n";
	for (Key key : ks)
	{
		std::cout << "\n  " << key << ":";
		std::cout << temp.value(key).show();
	}
	
	

	
	//std::cout<<
	/*std::cout << "\n  Creating and saving NoSqlDb elements with int data";
	std::cout << "\n ----------------------------------------------------\n";
	11
	NoSqlDb<intData> idb;	

	Element<intData> ielem1;
	ielem1.name = "elem1";
	ielem1.category = "test";
	ielem1.data = 1;

	idb.save(ielem1.name, ielem1);

	Element<intData> ielem2;
	ielem2.name = "elem2";
	ielem2.category = "test";
	ielem2.data = 2;

	idb.save(ielem2.name, ielem2);

	Element<intData> ielem3;
	ielem3.name = "elem3";
	ielem3.category = "test";
	ielem3.data = 3;

	idb.save(ielem3.name, ielem3);

	Element<intData> ielem4;
	ielem3.name = "elem4";
	ielem3.category = "test";
	ielem3.data = 4;

	idb.save(ielem3.name, ielem3);

	std::cout << "\n  Retrieving elements from NoSqlDb<int>";
	std::cout << "\n ---------------------------------------\n";

	std::cout << "\n  size of idb = " << idb.count() << "\n";
	Keys ikeys = idb.keys();
	for (Key key : ikeys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << idb.value(key).show();
	}
	std::cout << "\n\n";*/
}
#endif
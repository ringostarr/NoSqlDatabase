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
Provide QUery Engine which essentially contains store procedures to provide query support for database

Public Interface:
=================
class QueryEngine
			Speckey():query for specific key req 7.1
			childkey():query for children req 7.2
			itemstr():query for string matching item req 7.4
			catstr() : query for string matching category 7.5
			datastr():query for string matching in string data rq 7.6
			timestr():query for elements between dates req 7.7
			regexstr(): query for regular expression part 1
			regexstr2(): query for regular expression part 2 - total exceeds 50 lines so have to divide.
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
#include "NoSqlDb.h"
#include <regex>
template<typename T>
class QueryEngine
{
public:
	NoSqlDb<T> speckey(NoSqlDb<T> db, std::string key)
	{
		NoSqlDb<T> newdb;
	    Key abc;
		Keys ks = db.keys();
		abc = key;
		for (Key k : ks)
		{
			if (k == key)
			{
				Element<T> e;
				e = db.value(k).ret();
				newdb.save(k, e);
			}
				//newdb.save(k,)
		}
		std::cout<<newdb.value(abc).show();
			
			
		return newdb;
	
	}
	NoSqlDb<T> childkey(NoSqlDb<T> db, std::string childname)
	{
		NoSqlDb<T> newdb;			
			Keys ks = db.keys();
			for (Key k : ks)
			{
				Element<T> e=db.value(k).ret();
				
				std::vector<std::string> v = e.childrel;
				if (std::find(v.begin(), v.end(), childname) != v.end())
				{
					newdb.save(e.name, e);
				std::cout<<	newdb.value(k).show();
				}
			}
			return newdb;
		
		
	}
	NoSqlDb<T> itemstr(NoSqlDb<T> db, std::string istr)
	{
		
		NoSqlDb<T> newdb;
		int c = db.count();
	
	Keys ks = db.keys();
	for (Key k:ks)
	{
		Element<T> e;
		e = db.value(k).ret();
		std::string chck = e.name;
		if (chck.find(istr) !=std::string::npos)
		{
			newdb.save(e.name, e);
			std::cout << newdb.value(k).show();
		}	
	}
	
	return newdb;
		
	}
	NoSqlDb<T> catstr(NoSqlDb<T> db, std::string istr)
	{
		
		NoSqlDb<T> newdb;
		size_t c = db.count();
		
		Keys ks = db.keys();
		for (Key k : ks)
		{
			
			Element<T> e;
			e = db.value(k).ret();
			size_t p;
			std::string cr = e.category.getValue();
			std::string tok;
			std::string dl = "\n       ";
			std::string chck;
			tok = cr;
			while ((p = cr.find(dl)) != std::string::npos)
			{
				tok = cr.substr(0, p);
				cr = tok;
				
			}
			if (cr.find(istr) != std::string::npos)
			{
				newdb.save(e.name, e);
				std::cout << newdb.value(k).show();
			}

		}
		return newdb;
	}

	NoSqlDb<std::string> datastr(NoSqlDb<std::string> db,std::string istr)
	{
		NoSqlDb<T> newdb;
		size_t c = db.count();
		//std::cout << c;
		Keys ks = db.keys();
		for (Key k : ks)
		{
			//std::cout << k;
			Element<T> e;
			e = db.value(k).ret();
			std::string chck = e.data;
			if (chck.find(istr) != std::string::npos)
			{
				newdb.save(e.name, e);
				std::cout << newdb.value(k).show();
			}

			
		}
		return newdb;

	
	}
	
	NoSqlDb<T> timestr(NoSqlDb<T> db, std::string x, std::string y ="")
	{
		NoSqlDb<T> newdb;
		Keys ks = db.keys();
		for (Key k : ks)
		{
			Element<T> e;
			e=db.value(k).ret();
			std::string a = e.timeDate.getValue();
			//time_t t = std::mktime(a);
			if (y.empty())
			{
				if (a >= x)
				{
					newdb.save(e.name, e);
					std::cout << newdb.value(k).show();
				}
			}
			else
			{
				if (a >= x  && a <= y)
				{
					newdb.save(e.name, e);
					std::cout << newdb.value(k).show();
				}
			}


		}
		return newdb;
	}
	template<typename X>
	NoSqlDb<T> regExstr(NoSqlDb<T> db, X fieldname, std::string pattern)
	{
		std::regex pattern1(pattern);
		NoSqlDb<T> newdb;
		Keys ks = db.keys();
		std::string str;
		for (Key k : ks)
		{
			Element<T> e;
			e=db.value(k).ret();
			if (fieldname == "name")
			{
				str = e.name;
				if (std::regex_match(str, std::regex(pattern)))
				{
					newdb.save(e.name, e);
					std::cout << newdb.value(k).show();
				}
			}
			else if (fieldname == "key")
			{
				str = k;
				if (std::regex_match(k, std::regex(pattern)))
				{
					newdb.save(k, e);
					std::cout << newdb.value(k).show();
				}
			}
		}
		return newdb;
	}
	template<typename X>
	NoSqlDb<T> regExstr2(NoSqlDb<T> db, X fieldname, std::string pattern)
	{
		std::regex pattern1(pattern);
		NoSqlDb<T> newdb;
		Keys ks = db.keys();
		std::string str;
		for (Key k : ks)
		{
			Element<T> e;
			e = db.value(k).ret();
			if (fieldname == "category")
			{
				str = e.category.getValue();
				if (std::regex_match(str, std::regex(pattern)))
				{
					newdb.save(e.name, e);
					std::cout << newdb.value(k).show();
				}
			}
			else if (fieldname == "description")
			{
				str = e.description.getValue;
				if (std::regex_match(k, std::regex(pattern)))
				{
					newdb.save(k, e);
					std::cout << newdb.value(k).show();
				}
			}
		}
		return newdb;
	}
	NoSqlDb<T> keyunion(NoSqlDb<T> db1, NoSqlDb<T> db2)
	{
		NoSqlDb<T> newdb;
		Keys ks1 = db1.keys();
		Keys ks2 = db2.keys();
		for (Key k : ks1)
		{
			Element<T> e;
			e = db1.value(k).ret();
			newdb.save(k, e);
		}
		for (Key k : ks2)
		{
			Element<T> e;
			e = db2.value(k).ret();
			newdb.save(k, e);
		}
		Keys nk = newdb.keys();
		for (Key k : nk)
		{
			std::cout << newdb.value(k).show();
		}
		return newdb;
	}
	
};


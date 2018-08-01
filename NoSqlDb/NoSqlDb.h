#pragma once
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
Define ELemet class , Database Class , DatabaseWrapper class
database objects and its operations like saving ,persisting to xml etc.

Public Interface:
=================
Class Element<T>:
			show(): Display data+metadata
			ret() : returns element
			edit() : edits element
Class NoSqlDB<T>
			Value(key):returns element at key
			save(): Save db element to object
			Keys() : returns collection of keys
			count(): returns count of items in db
Class DBWrapper<T>
			DBWrapper() : default constructor
			DBWrapper(&db) : copy constructor
			operator =() : overloaded operator
		std::string save(): persist to xml file
			restore(): restore from file

Build Process:
==============
Required files
- CppProperties.h,XmlElement.h,XmlDocument.h
Build commands (either one)
- devenv Project1HelpS06.sln
- cl /EHsc /test_nosqldb NoSqlDb.cpp /link setargv.obj

Maintenance History:
====================
ver1.0 : Initial commit- Taken from Professor Fawcett

ver 1.1 :29th January,2017
   -Added DBWrapper for persistance
   -Added save and restore functions to copy and restore from file
   -Edited ELement class to include all requirements
*/

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include "../oodp1/CppProperties.h"
#include <vector>
#include<fstream>
#include "../Convert/Convert.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement.h"

using namespace XmlProcessing;
/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair
// - it needs to store child data, something for you to implement
//
class Widget
{
public:
	int wint;
	std::string wstr;
	std::ostream& operator<<(std::ostream& os)
	{
		os << "wint:" << wint << "\n";
		os << "wstr:" << wstr << "\n";
		return os;
	}
};
template<typename Data>
class Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using TimeDate = std::string;

	Property<Name> name;            // metadata
	Property<Category> category;    // metadata
	Property<std::string> timeDate;    // metadata
	std::vector<std::string> childrel; //metadata - child relationships
	Property<Data> data;            // data
	Property<std::string> description; //metadata
	std::string show();
	Element<Data> ret();
	Element<Data> edit(Element<Data>);
	//~Element<Data>();
};

template<typename Data>
std::string Element<Data>::show()
{
	// show children when you've implemented them

	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "Description" << " : " << description;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	//int iter;
	out << "\n";
	out << "Children:";
	std::vector<std::string> c = childrel;
	for (std::vector<std::string>::const_iterator iter = c.begin();iter != c.end(); ++iter)
		out << *iter << "\n";
	out << "\n";
	return out.str();
}



template<typename Data>
Element<Data> Element<Data>::ret()
{
	Element<Data> e;
	e.name = name;
	e.category = category;
	e.timeDate = timeDate;
	e.data = data;
	e.description = description;
	e.childrel = childrel;
	return e;
}
template<typename Data>
Element<Data> Element<Data>::edit(Element<Data> e)
{
	name = e.name;
	category = e.category;
	timeDate = e.timeDate;
	data = e.data;
	description = e.description;
	childrel = e.childrel;
	return e;

}
/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements
// - you will need to add query capability
//   That should probably be another class that uses NoSqlDb
//
template<typename Data>
class NoSqlDb
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;
	//Element<Data> retele();
	Keys keys();
	bool save(Key key, Element<Data> elem);
	bool edit(Key key, Element<Data> elem);
	bool del(Key key);
	Element<Data> value(Key key);
	size_t count();
private:
	using Item = std::pair<Key, Element<Data>>;

	std::unordered_map<Key, Element<Data>> store;
};

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}
template<typename Data>
bool NoSqlDb<Data>::del(Key key)
{
	if (store.find(key) == store.end())
	{
		std::cout << "No element exists\n";
		return false;
	}
	else {
		store.erase(key);
		return true;
	}
}
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
	if (store.find(key) != store.end())
		return false;
	store[key] = elem;
	return true;
}
template<typename Data>
bool NoSqlDb<Data>::edit(Key key, Element<Data> elem)
{
	if (store.find(key) != store.end())
	{
		store[key] = elem;
		
		return true;
	}
	else
		return false;
}
template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}

template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.size();
}


/////////////////////////////////////////////////////////////////////
// Persistance class
// - you will need to implement that using the XmlDocument class
//   from Handouts/Repository


using SPtr = std::shared_ptr<AbstractXmlElement>;
template<typename X>
class DBWrapper 
{
public:
	DBWrapper() {};
	DBWrapper(const NoSqlDb<X> &db) { ndb = db; };
	DBWrapper& operator=(const DBWrapper<X> &db);
	std::string save(std::string xml)
	{
		XmlDocument doc;
		SPtr root = makeTaggedElement("Database");
		doc.docElement() = root;
		Element<X> e;
		Keys ks = ndb.keys();//Keys();
		for (Key k : ks)
		{
			SPtr item = makeTaggedElement("Item");
			root->addChild(item);
			std::string keyname = k;
			SPtr key = makeTaggedElement("Key");
			item->addChild(key);
			key->addChild(makeTextElement(k));
			e = ndb.value(k).ret();
			std::string itemname = e.name;
			std::string catname = e.category;
			std::string datetime;
			//datetime =std::stoi(e.timeDate);
			datetime = e.timeDate;
			//std::cout << e.timeDate;
			std::string desc = e.description;
			X data = e.data;
			SPtr iname = makeTaggedElement("itemname");
			SPtr cname = makeTaggedElement("category");
			SPtr dt = makeTaggedElement("DateTime");
			SPtr des = makeTaggedElement("description");
			SPtr dat = makeTaggedElement("Data");
			SPtr child = makeTaggedElement("Children");
			std::vector<std::string> crel;
			crel= e.childrel;
			
			for (std::vector<std::string>::const_iterator iter = crel.begin(); iter != crel.end(); ++iter)
			{
				child->addChild(makeTextElement(*iter));
			}
			iname->addChild(makeTextElement(itemname));
			cname->addChild(makeTextElement(e.category.getValue()));
			dt->addChild(makeTextElement(e.timeDate.getValue()));
			dat->addChild(makeTextElement(e.data.getValue()));
			des->addChild(makeTextElement(e.description.getValue()));
			SPtr Ele = makeTaggedElement("Value");
			item->addChild(Ele);
			Ele->addChild(iname);
			Ele->addChild(cname);
			Ele->addChild(dt);
			Ele->addChild(des);
			Ele->addChild(dat);
			Ele->addChild(child);
		}
		std::ofstream myfile;
		myfile.open(xml);
		myfile << doc.toString();
		myfile.close();
		return doc.toString();
	}
	NoSqlDb<X> restore(const std::string &xml)
	{
		NoSqlDb<X> tempdb;
		XmlDocument doc(xml,XmlDocument::file);
		std::vector<SPtr> elems = doc.element("Key").select();
		for (unsigned int i=0;i<elems.size();i++)
		{
			std::string tempkey = Convert<std::string>::fromString(elems[i]->children()[0]->value());
			elems = doc.element("itemname").select();
			std::string tempitemname= Convert<std::string>::fromString(elems[i]->children()[0]->value());
			elems = doc.element("category").select();
			std::string tempcatname = elems[i]->children()[0]->value();
			elems = doc.element("DateTime").select();
			std::string tempdatetime= elems[i]->children()[0]->value();
			elems = doc.element("description").select();
			std::string tempdesc=elems[i]->children()[0]->value();
			X Data = Convert<X>::fromString(elems[i]->children()[0]->value());
			elems = doc.element("Children").select();
			std::string dl = "\n          ";
			size_t p;
			std::string tok;
			std::vector<std::string> crel;
			if (elems[i]->children().size() != 0) {
				std::string cr = elems[i]->children()[0]->value();
				while ((p = cr.find(dl)) != std::string::npos)
				{
					tok = cr.substr(0, p);
					crel.push_back(tok);
					cr.erase(0, p + dl.length());
				}
			}
			Element<X> e;
			e.name =tempitemname;
			e.category = tempcatname;
			e.description = tempdesc;
			e.timeDate = tempdatetime;
			e.data = Data;
			e.childrel = crel;
			tempdb.save(tempkey, e);
			elems = doc.element("Key").select();
		}
		return tempdb;
	}
	
private:
	NoSqlDb<X> ndb;

};
template<typename X>
DBWrapper<X>& DBWrapper<X>::operator=(const DBWrapper<X>& db)
{
	ndb = db;
	return *this;
}

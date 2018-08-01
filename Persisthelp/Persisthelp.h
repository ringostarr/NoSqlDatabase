#pragma once
/////////////////////////////////////////////////////////////////////
// PersistHelp.h - demo one way to persist objects to XML file     //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////
/*
* This package builds wrapper classes for strings, doubles, and Widgets.
* They provide the same functionality as the wrapped type, but also
* attach methods save() and restore() to persist the wrapped type.
*
* Package operations:
* -------------------
* This package contains:
* - IPersist interface - defines the methods needed to
*   serialize to XML and deserialize from XML
* - StringWrapper class that persists strings
* - DoubleWrapper class that persists doubles
* - Widget class with int and string properties
* - WidgetWrapper class that persits Widgets
*
* Required Files:
* ---------------
*   PersistHelp.h, PersistHelp.cpp,
*   Convert.h, Convert.cpp,
*   CppProperties.h, CppProperties.cpp,
*   XmlDocument.h, XmlDocument.cpp, XmlElement.h, XmlElement.cpp,
*  ---- required to build XmlDocument from file or string ----
*   XmlParser.h, XmlParser.cpp,
*   XmlElementParts.h, XmlElementParts.cpp,
*   ITokenizer.h, Tokenizer.h, Tokenizer.cpp,
*
* Build Instructions:
* -------------------
* - Uses XmlDocument, so build XmlDocument project as static library
*   after undefining its test stub
*/
#include <string>
#include <vector>
#include "../Convert/Convert.h"
#include "../oodp1/CppProperties.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement.h"
#include "../NoSqlDb/NoSqlDb.h"
using namespace XmlProcessing;

/////////////////////////////////////////////////////////////////////
// IPersist interface
// - declares methods Persistance Wrappers must use
//
template<typename T>
struct IPersist
{
	using Xml = std::string;

	virtual ~IPersist() {}
	virtual IPersist<T>& operator=(const T& t) = 0;
	virtual operator T() = 0;
	virtual Xml save() = 0;
	virtual void restore(const Xml& xml) = 0;
	std::string id() { return typeid(*this).name(); }
};
using SPtr = std::shared_ptr<AbstractXmlElement>;
/////////////////////////////////////////////////////////////////////
// StringWrapper class
// - wrapper classes implement the persistance interface
// - they almost certainly should be non-template classes
//   because they have to be type specific
//
template<typename X>
class DBWrapper : public IPersist<Element<X>>
{
public:
	DBWrapper() {};
	DBWrapper(const DBWrapper<X> &db) :DBWrap_(db) {};
	DBWrapper& operator=(const DBWrapper<X> &db);
	std::string save()
	{
		XmlDocument doc;
			SPtr root = makeTaggedElement("Database");
			doc.docElement() = root;
			Element<X> e;
			Keys ks = ndb.Keys();
			for (Key k : ks)
			{
				SPtr item = makeTaggedElement("Item");
				root->addChild(item);
				std::string keyname = k;
				SPtr key = makeTaggedElement("Key");
				item->addChild(key);
				e = ndb.value(k).ret();
				std::string itemname=e.name;
				std::string catname=e.category;
				std::string datetime=e.timeDate;
				std::string desc = e.description;
				X data = e.data;
				SPtr iname = makeTaggedElement("item");
				SPtr cname = makeTaggedElement("category");
				SPtr dt = makeTaggedElement("DateTime");
				SPtr des = makeTaggedElement("description");
				SPtr dat = makeTaggedElement("Data");
				iname->addChild(makeTextElement(itemname));
				cname->addChild(makeTextElement(catname));
				dt->addChild(makeTextElement(datetime));
				dat->addChild(makeTextElement(data));
				des->addChild(makeTextElement(desc));
				SPtr Ele = makeTaggedElement("Value");
				item->addChild(Ele);
				Ele->addChild(iname);
				Ele->addChild(cname);
				Ele->addChild(dt);
				Ele->addChild(des);
				Ele->addChild(dat);
				for (std::string x : e.childrel)
				{

				}


			}
			return doc.toString();
	}
	void restore(const Xml &xml);
	operator DBWrapper();
private:
	NoSqlDb<X> ndb;

};
template<typename X>
DBWrapper<X>& DBWrapper<X>::operator=(const DBWrapper<X>& db)
{
	ndb = db;
	return *this;
}
//template<typename X>
//inline DBWrapper<X>::Xml DBWrapper<X>::save()
//{
//	XmlDocument doc;
//	SPtr root = makeTaggedElement("Database");
//	doc.docElement() = root;
//	Element<X> e;
//	//Keys ks = ndb.Keys();
//
//}

class StringWrapper : public IPersist<std::string>
{
public:
	StringWrapper(const std::string& str = "") : str_(str) {}
	StringWrapper& operator=(const std::string& s);  // covariant return type
	Xml save();
	void restore(const Xml& xml);
	operator std::string();
private:
	std::string str_;
};
//----< assignment overload >----------------------------------------

StringWrapper& StringWrapper::operator=(const std::string& s)
{
	str_ = s;
	return *this;
}
//----< saves instance state as XML string >-------------------------

inline StringWrapper::Xml StringWrapper::save()
{
	return "<StringWrapper>" + str_ + "</StringWrapper>";
}
//---< restores instance state from XML string >---------------------

inline void StringWrapper::restore(const Xml& xml)
{
	size_t posStart = xml.find_first_of(">") + 1;
	size_t posEnd = xml.find_last_of("</");
	str_ = xml.substr(posStart, posEnd - posStart - 1);
}

//----< cast operator returns string member >------------------------

inline StringWrapper::operator std::string()
{
	return str_;
}

/////////////////////////////////////////////////////////////////////
// DoubleWrapper class
//
class DoubleWrapper : public IPersist<double>
{
public:
	DoubleWrapper(double d = 0) : d_(d) {}
	DoubleWrapper& operator=(const double& d);
	Xml save();
	void restore(const Xml& xml);
	operator double();
private:
	double d_;
};
//----< assignment overload >----------------------------------------

DoubleWrapper& DoubleWrapper::operator=(const double& d)
{
	d_ = d;
	return *this;
}
//----< saves instance state as XML string >-------------------------

inline DoubleWrapper::Xml DoubleWrapper::save()
{
	return "<DoubleWrapper>" + Convert<double>::toString(d_) + "</DoubleWrapper>";
}
//---< restores instance state from XML string >---------------------

inline void DoubleWrapper::restore(const Xml& xml)
{
	size_t posStart = xml.find_first_of(">") + 1;
	size_t posEnd = xml.find_last_of("</");
	std::string bodyString = xml.substr(posStart, posEnd - posStart - 1);
	d_ = Convert<double>::fromString(bodyString);
}
//----< cast operator returns double member >------------------------

inline DoubleWrapper::operator double()
{
	return d_;
}

/////////////////////////////////////////////////////////////////////
// Widget and WidgetWrapper classes
//
class Widget
{
public:
	Property<int> wint;
	Property<std::string> wstr;
};

class WidgetWrapper : public IPersist<Widget>
{
public:
	WidgetWrapper() {};
	WidgetWrapper(Widget widget) : widget_(widget) {}
	WidgetWrapper& operator=(const Widget& widget);
	Xml save();
	void restore(const Xml& xml);
	operator Widget();
private:
	Widget widget_;
};
//----< assignment operator overload >-------------------------------

WidgetWrapper& WidgetWrapper::operator=(const Widget& widget)
{
	widget_ = widget;
	return *this;
}



//----< saves instance state as XML string >-------------------------

inline WidgetWrapper::Xml WidgetWrapper::save()
{
	XmlDocument doc;

	// make and add root element to doc
	SPtr root = makeTaggedElement("WidgetWrapper");
	doc.docElement() = root;

	// make Property<int> wint element and add to root
	int wintValue = static_cast<Widget>(*this).wint;
	std::string wintStr = Convert<int>::toString(wintValue);
	SPtr pWintElem = makeTaggedElement("wint");
	pWintElem->addChild(makeTextElement(wintStr));
	root->addChild(pWintElem);

	// make Property<std::string> wstr element and add to root
	std::string wStr = static_cast<Widget>(*this).wstr;
	SPtr pWstrElem = makeTaggedElement("wstr");
	pWstrElem->addChild(makeTextElement(wStr));
	root->addChild(pWstrElem);
	return doc.toString();
}
//---< restores instance state from XML string >---------------------

inline void WidgetWrapper::restore(const Xml& xml)
{
	XmlDocument doc(xml);
	std::vector<SPtr> elems = doc.element("wint").select();
	std::string temp = elems[0]->children()[0]->value();
	widget_.wint = Convert<int>::fromString(temp);
	elems = doc.element("wstr").select();
	widget_.wstr = elems[0]->children()[0]->value();
}
//----< cast operator returns Widget member >------------------------

inline WidgetWrapper::operator Widget()
{
	return widget_;
}

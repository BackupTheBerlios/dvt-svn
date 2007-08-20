#include "dvtLanguageProfile.h"

#include "dvtCore.h"
#include "dvtExceptions.h"

#include "dvtLanguage.h"
#include "dvtLanguage_en.h"
#include "dvtLanguage_de.h"
#include "dvtLanguage_fr.h"
#include "dvtLanguage_fi.h"

#include "debug.h"
#include "xmlNode.h"

#include "assert.h"

#include <fstream>

namespace Dvt
{

using namespace std;

/* Case ***********************************************************************/

Case::Case(LanguageProfile* lp)
{
	p_langProfile = lp;
}

Case::~Case()
{
}

string Case::id() {return p_id;}
MlString& Case::name() {return p_name;}

string Case::getDefArticle(Gender::Type gen)
{
	map<Gender::Type, string>::iterator it;
	
	it = p_defArticles.find(gen);
	if (it != p_defArticles.end())
		return it->second;
	else
		return string();
}

string Case::getDefArticlePl(Gender::Type gen)
{
	map<Gender::Type, string>::iterator it;
	
	it = p_defArticlesPl.find(gen);
	if (it != p_defArticlesPl.end())
		return it->second;
	else
		return string();
}

string Case::getIndefArticle(Gender::Type gen)
{
	map<Gender::Type, string>::iterator it;
	
	it = p_indefArticles.find(gen);
	if (it != p_indefArticles.end())
		return it->second;
	else
		return string();
}

string Case::getIndefArticlePl(Gender::Type gen)
{
	map<Gender::Type, string>::iterator it;
	
	it = p_indefArticlesPl.find(gen);
	if (it != p_indefArticlesPl.end())
		return it->second;
	else
		return string();
}

void Case::setFromXmlNode(sxml::XmlNode* node)
{
	p_id = node->attributes["t"];
	if (p_id.empty()) {
		DEBUG(DBG_DLP, "ERROR: Case::setFromXmlNode() no id!");
		throw EXmlMissingAttribute("<stream>", node->name, "t");
	}
		
	sxml::XmlNode* n = node->findFirst("name");
	if (n != NULL) {
		p_name.setFromXmlNode(n);
		DEBUG(DBG_DLP, "Case %s: %s", p_id.c_str(), p_name.c_str());
		
	} else {
		DEBUG(DBG_DLP, "ERROR: Case::setFromXmlNode(): no name!");
		throw EXmlTreeInvalid("<stream>", node->name);
		
	}
	
	sxml::XmlNode* articlesNode = node->findFirst("articles");
	if (articlesNode != NULL) {
		sxml::XmlNode* n = articlesNode->findFirst("definite");
		if (n != NULL) {
			sxml::NodeSearch* ns = n->findInit("a");
			sxml::XmlNode* a = n->findNext(ns);
			while (a != NULL) {
				if (a->children.size() == 1 && 
					a->children[0]->type == sxml::ntTextNode)
				{
					Gender::Type gt = Gender::getType(a->attributes["g"]);
					string numerus = a->attributes["n"];
					if (numerus == "sg")
						p_defArticles[gt] = a->children[0]->name;
					else if (numerus == "pl")
						p_defArticlesPl[gt] = a->children[0]->name;
					else
						DEBUG(DBG_DLP, "WARNING: Case::setFromXmlNode(): Missing attribute \"n\" in node \"a\"");
						
				}
				
				a = n->findNext(ns);
				
			}
			
			n->findFree(ns);
			
		}
		
		n = articlesNode->findFirst("indefinite");
		if (n != NULL) {
			sxml::NodeSearch* ns = n->findInit("a");
			sxml::XmlNode* a = n->findNext(ns);
			while (a != NULL) {
				if (a->children.size() == 1 && 
					a->children[0]->type == sxml::ntTextNode)
				{
					Gender::Type gt = Gender::getType(a->attributes["g"]);
					string numerus = a->attributes["n"];
					if (numerus == "sg")
						p_indefArticles[gt] = a->children[0]->name;
					else if (numerus == "pl")
						p_indefArticlesPl[gt] = a->children[0]->name;
					else
						DEBUG(DBG_DLP, "WARNING: Case::setFromXmlNode(): Missing attribute \"n\" in node \"a\"");
						
				}
				
				a = n->findNext(ns);
				
			}
			
			n->findFree(ns);
			
		}
		
	} // articlesNode

}

void Case::setToXmlNode(sxml::XmlNode* node)
{
	assert(false);
}

/* Conjugation ****************************************************************/

Conjugation::Conjugation(LanguageProfile* lp)
{
	p_langProfile = lp;
}

Conjugation::~Conjugation()
{
}

string Conjugation::id() {return p_id;}
MlString& Conjugation::name() {return p_name;}

void Conjugation::setFromXmlNode(sxml::XmlNode* node)
{
	p_id = node->attributes["t"];
	if (p_id.empty()) {
		DEBUG(DBG_DLP, "ERROR: Conjugation::setFromXmlNode() no id!");
		throw EXmlMissingAttribute("<stream>", node->name, "t");
	}
		
	sxml::XmlNode* n = node->findFirst("name");
	if (n != NULL)
		p_name.setFromXmlNode(n);
	else {
		DEBUG(DBG_DLP, "ERROR: Conjugation::setFromXmlNode(): no name!");
		throw EXmlTreeInvalid("<stream>", node->name);
	}
}

void Conjugation::setToXmlNode(sxml::XmlNode* node)
{
	assert(false);
}

/* LanguageProfile ************************************************************/

LanguageProfile::LanguageProfile()
{
	core = Core::getInstance();
	p_genders.push_back(Gender::Undefined);
	p_lang = NULL;
}

LanguageProfile::~LanguageProfile()
{
}

string LanguageProfile::langCode() {return p_langCode;}
string LanguageProfile::version() {return p_version;}
MlString LanguageProfile::name() {return p_name;}

Language* LanguageProfile::lang() {return p_lang;}

vector<Gender::Type>& LanguageProfile::genders() {return p_genders;}
map<string, Case*>& LanguageProfile::cases() {return p_cases;}
map<string, Conjugation*>& LanguageProfile::conjugations() {return p_conjugations;}

Case* LanguageProfile::getCase(std::string id)
{
	map<std::string, Case*>::const_iterator it;
	
	it = p_cases.find(id);
	if (it != p_cases.end())
		return it->second;
	else	
		return NULL;
}

Conjugation* LanguageProfile::getConjugation(std::string id)
{
	map<std::string, Conjugation*>::const_iterator it;
	
	it = p_conjugations.find(id);
	if (it != p_conjugations.end())
		return it->second;
	else	
		return NULL;
}

string LanguageProfile::getPersonalPronoun(PersonalPronoun pp)
{
	map<PersonalPronoun, std::string>::iterator it;
	
	it = p_personalPronouns.find(pp);
	if (it != p_personalPronouns.end())
		return it->second;
	else
		return string();
}

string LanguageProfile::getDefArticle(Gender::Type gen)
{
	map<Gender::Type, string>::iterator it;
	
	it = p_defArticles.find(gen);
	if (it != p_defArticles.end())
		return it->second;
	else
		return string();
}

void LanguageProfile::readFromFile(string fileName)
{
	p_fileName = fileName;
	 
	ifstream fin(p_fileName.c_str());
		
	if (fin.fail()) {
		DEBUG(DBG_DLP, "LanguageProfile::readFromFile(): could not open file %s\n", fileName.c_str());
		throw EFileError(p_fileName);
	}
	
	try {
		readFromStream(fin);
		fin.close();
		
	} catch (EFileException e) {
		e.fileName = p_fileName;
		fin.close();
		throw e;
		
	} catch (...) {
		fin.close();
		throw;
		
	}
}

void LanguageProfile::readFromStream(istream& readFrom)
{
	sxml::XmlNode* xmlNode = new sxml::XmlNode();
	
	try {
		xmlNode->readFromStream(readFrom, true);
		
	} catch (sxml::Exception e) {
		delete xmlNode;
		DEBUG(DBG_DLP, "LanguageProfile::readFromStream(): eXmlParsingError %i\n", e);
		
		throw EXmlParsingError(p_fileName);
	}
	
	sxml::XmlNode* dlpNode;
	sxml::XmlNode* node;
	sxml::XmlNode* node2;
	sxml::NodeAttributesIterator ait;
	
	try {
		// get root
		dlpNode = xmlNode->findFirst("languageProfile", true);
		if (NULL == dlpNode) throw EXmlRootInvalid(p_fileName, "languageProfile");
		
		ait = dlpNode->attributes.find("formatVersion");
		if (ait == dlpNode->attributes.end()) 
			throw EXmlMissingAttribute(p_fileName, "languageProfile", "formatVersion");
		if (DVT_DLP_XML_FORMATVERSION != ait->second) 
			throw EXmlUnsupportedFormatVersion(p_fileName, ait->second);
		
		ait = dlpNode->attributes.find("langCode");
		if (ait == dlpNode->attributes.end()) 
			throw EXmlMissingAttribute(p_fileName, "languageProfile", "langCode");
		p_langCode = ait->second;
		
		if (p_langCode == "en") {
			p_lang = new Language_en(this);
		} else if (p_langCode == "de") {
			p_lang = new Language_de(this);
		} else if (p_langCode == "fr") {
			p_lang = new Language_fr(this);
		} else if (p_langCode == "fi") {
			p_lang = new Language_fi(this);
		} else {
			p_lang = new Language(this);
		}
		
		ait = dlpNode->attributes.find("version");
		if (ait != dlpNode->attributes.end())
			p_version = ait->second;
		
		node = dlpNode->findFirst("name");
		if (node == NULL) throw EXmlMissingNode(p_fileName, "name");
		p_name.setFromXmlNode(node);
		
		node = dlpNode->findFirst("conjugations");
		if (node != NULL) {
			sxml::NodeSearch* ns = node->findInit("defcj");
			node2 = node->findNext(ns);
			try {
				while (node2 != NULL) {
					Conjugation* conj = new Conjugation(this);
					conj->setFromXmlNode(node2);
					p_conjugations[conj->id()] = conj;
					node2 = node->findNext(ns);
				}
			} catch (Exception e) {
				node->findFree(ns);
				throw;
			}
			node->findFree(ns);
		}
		
		node = dlpNode->findFirst("cases");
		if (node != NULL) {
			sxml::NodeSearch* ns = node->findInit("c");
			node2 = node->findNext(ns);
			try {
				while (node2 != NULL) {
					Case* c = new Case(this);
					c->setFromXmlNode(node2);
					p_cases[c->id()] = c;
					node2 = node->findNext(ns);
				}
			} catch (Exception e) {
				node->findFree(ns);
				throw;
			}
			node->findFree(ns);
		}
		
		node = dlpNode->findFirst("genders");
		if (node != NULL) {
			sxml::NodeSearch* ns = node->findInit("g");
			node2 = node->findNext(ns);
			while (node2 != NULL) {
				Gender::Type gt = Gender::getType(node2->attributes["t"]);
				if (gt > Gender::Undefined && gt < Gender::High)
					p_genders.push_back(gt);
				node2 = node->findNext(ns);
			}
			node->findFree(ns);
		}
		
		node = dlpNode->findFirst("defArticles");
		if (node != NULL) {
			sxml::NodeSearch* ns = node->findInit("g");
			node2 = node->findNext(ns);
			while (node2 != NULL) {
				if (node2->children.size() == 1 && 
					node2->children[0]->type == sxml::ntTextNode)
				{
					Gender::Type gt = Gender::getType(node2->attributes["t"]);
					p_defArticles[gt] = node2->children[0]->name;
				} 
				node2 = node->findNext(ns);
			}
			node->findFree(ns);
		}
		
		node = dlpNode->findFirst("personalPronouns");
		if (node != NULL) {
			sxml::NodeSearch* ns = node->findInit("p");
			node2 = node->findNext(ns);
			while (node2 != NULL) {
				PersonalPronoun pp = (PersonalPronoun) atoi(node2->attributes["t"].c_str());
				if (pp > ppUndefined && pp < ppHigh &&
					node2->children.size() > 0 && 
					node2->children[0]->type == sxml::ntTextNode) 
				{
					p_personalPronouns[pp] = node2->children[0]->name;
				}
				node2 = node->findNext(ns);
			}
			node->findFree(ns);
		}
		
		node = dlpNode->findFirst("translations");
		if (node != NULL) {
			node2 = node->findFirst("genders");
			if (node2 != NULL)
				Gender::updateFromXmlNode(node2);
			
			node2 = node->findFirst("gendersShort");
			if (node2 != NULL)
				Gender::updateShortNamesFromXmlNode(node2);
			
			node2 = node->findFirst("wordClasses");
			if (node2 != NULL)
				WordClass::updateFromXmlNode(node2);
				
			node2 = node->findFirst("wordClassesShort");
			if (node2 != NULL)
				WordClass::updateShortNamesFromXmlNode(node2);
		}
		
	} catch (EXmlTreeInvalid e) {
		DEBUG(DBG_DLP, "LanguageProfile::readFromStream(): Exception occurred: %s\n", e.msg.c_str());
		delete xmlNode;
		throw;
		
	} catch (Exception e) {
		DEBUG(DBG_DLP, "LanguageProfile::readFromStream(): Exception occurred: %s\n", e.msg.c_str());
		delete xmlNode;
		throw;
	}
	
	delete xmlNode;
}

void LanguageProfile::writeToFile(string fileName)
{
	assert(false);
}

void LanguageProfile::writeToStream(ostream& writeTo)
{
	assert(false);
}

void LanguageProfile::write()
{
	assert(false);
}

}

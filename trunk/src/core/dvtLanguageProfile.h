#ifndef DVTLANGUAGEPROFILE_H_
#define DVTLANGUAGEPROFILE_H_

#include "dvtMlString.h"
#include "dvtWordClass.h"

#include <map>
#include <string>
#include <iostream>

namespace Dvt
{

#define DVT_DLP_XML_FORMATVERSION	"1.0"

class Core;
class Language;
class LanguageProfile;

typedef enum {
	ppUndefined = 0,
	ppSingular1 = 1,
	ppSingular2 = 2,
	ppSingular3 = 3,
	ppPlural1 = 4,
	ppPlural2 = 5,
	ppPlural3 = 6,
	ppHigh = 7
} PersonalPronoun;

class Case
{
protected:
	LanguageProfile* p_langProfile;
	std::string p_id;
	MlString p_name;
	
public:
	Case(LanguageProfile* lp);
	virtual ~Case();
	
	std::string id();
	MlString& name();
	
	void setFromXmlNode(sxml::XmlNode* node);
	void setToXmlNode(sxml::XmlNode* node);
	
};

class Conjugation
{
protected:
	LanguageProfile* p_langProfile;
	std::string p_id;
	MlString p_name;
	
public:
	Conjugation(LanguageProfile* lp);
	virtual ~Conjugation();
	
	std::string id();
	MlString& name();
	
	void setFromXmlNode(sxml::XmlNode* node);
	void setToXmlNode(sxml::XmlNode* node);
	
};

class LanguageProfile
{
private:
	Core* core;
	
	Language* p_lang;
	std::string p_fileName;
	std::string p_langCode;
	std::string p_version;
	MlString p_name;
	
	std::map<PersonalPronoun, std::string> p_personalPronouns;
	std::vector<Gender::Type> p_genders;
	std::map<Gender::Type, std::string> p_defArticles;
	std::map<std::string, Case*> p_cases;
	std::map<std::string, Conjugation*> p_conjugations;
	
public:
	LanguageProfile();
	virtual ~LanguageProfile();
	
	virtual std::string langCode();
	virtual std::string version();
	virtual MlString name();
	
	virtual Language* lang();
	
	virtual std::vector<Gender::Type>& genders();
	virtual std::map<std::string, Case*>& cases();
	virtual std::map<std::string, Conjugation*>& conjugations();
	
	virtual Case* getCase(std::string id);
	virtual Conjugation* getConjugation(std::string id);
	virtual std::string getPersonalPronoun(PersonalPronoun pp);
	virtual std::string getDefArticle(Gender::Type gen);
	
	virtual void readFromFile(std::string fileName);
	virtual void readFromStream(std::istream& readFrom);
	
	virtual void writeToFile(std::string fileName);
	virtual void writeToStream(std::ostream& writeTo);
	virtual void write();
	
};

}

#endif /*DVTLANGUAGEPROFILE_H_*/

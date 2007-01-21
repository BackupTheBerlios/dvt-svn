/** @file
 * 
 * Dvt::Lesson - Part of the portable Dictionary and Vocabulary Trainer.
 * 
 * Copyright (C) 2006  Denis Martin (http://www.delta-my.de/)
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 */

#include "dvtLesson.h"

#include "dvtCore.h"
#include "dvtExceptions.h"
#include "dvtLanguageProfile.h"

#include "debug.h"
#include "xmlNode.h"

#include <fstream>

namespace Dvt
{

using namespace std;

Lesson::Lesson(bool newLesson)
{
	core = Core::getInstance();
	p_langProfile_o = NULL;
	p_langProfile_t = NULL;
	p_metaRead = false;
	p_dataRead = false;
	
	if (newLesson) {
		p_metaRead = true;
		p_dataRead = true;
	}
}

Lesson::~Lesson()
{
}

LanguageProfile* Lesson::langProfile_o() {return p_langProfile_o;}
LanguageProfile* Lesson::langProfile_t() {return p_langProfile_t;}
std::string Lesson::version() {return p_version;}

void Lesson::setLangProfile_o(LanguageProfile* lp) {p_langProfile_o = lp;}
void Lesson::setLangProfile_t(LanguageProfile* lp) {p_langProfile_t = lp;}

bool Lesson::metaRead() {return p_metaRead;}
bool Lesson::dataRead() {return p_dataRead;}

MlString& Lesson::title() {return p_title;}
MlString& Lesson::description() {return p_description;}
MlString& Lesson::license() {return p_license;}

std::vector<TrainingEntry*>& Lesson::entries() {return p_entries;}

TrainingEntry* Lesson::addEntry()
{
	Entry* o = new Entry(p_langProfile_o);
	Entry* t = new Entry(p_langProfile_t);
	TrainingEntry* te = new TrainingEntry(o, t);
	p_entries.push_back(te);
	return te;
}

void Lesson::deleteEntry(TrainingEntry* te)
{
	TrainingEntry* my_te = NULL;
	vector<TrainingEntry*>::iterator it;
	for (it = p_entries.begin(); it != p_entries.end(); it++) {
		my_te = *it;
		if (my_te == te) break;
		
	}
	
	assert(it != p_entries.end());
	p_entries.erase(it);
	delete te;
}

void Lesson::readFromStream(std::istream& readFrom, const ReadFlag readFlag)
{
	sxml::XmlNode* xmlNode = new sxml::XmlNode();
	
	try {
		xmlNode->readFromStream(readFrom, true);
		
	} catch (sxml::Exception e) {
		delete xmlNode;
		DEBUG(DBG_GENERAL, "TrainingLesson::readFromStream(): eXmlParsingError %i\n", e);
		
		throw EXmlParsingError("<stream>");
	}
	
	sxml::XmlNode* dtlNode;
	sxml::XmlNode* node;
	sxml::NodeAttributesIterator ait;
	
	try {
		// get root
		dtlNode = xmlNode->findFirst("trainingLesson", true);
		if (NULL == dtlNode) 
			throw EXmlRootInvalid("<stream>", "trainingLesson");
		
		ait = dtlNode->attributes.find("formatVersion");
		if (ait == dtlNode->attributes.end()) 
			throw EXmlMissingAttribute("<stream>", "trainingLesson", "formatVersion");
		if (DVT_DTL_XML_FORMATVERSION != ait->second) 
			throw EXmlUnsupportedFormatVersion("<stream>", ait->second);
		
		// meta data
		if (readFlag != rfReadData || !p_metaRead) {
			ait = dtlNode->attributes.find("langCode_o");
			if (ait == dtlNode->attributes.end()) 
				throw EXmlMissingAttribute("<stream>", "trainingLesson", "langCode_o");
			p_langProfile_o = core->getLanguageProfile(ait->second);
			
			ait = dtlNode->attributes.find("langCode_t");
			if (ait == dtlNode->attributes.end()) 
				throw EXmlMissingAttribute("<stream>", "trainingLesson", "langCode_t");
			p_langProfile_t = core->getLanguageProfile(ait->second);
			
			ait = dtlNode->attributes.find("version");
			if (ait == dtlNode->attributes.end()) 
				throw EXmlMissingAttribute("<stream>", "trainingLesson", "version");
			p_version = ait->second;
			
			// get title
			node = dtlNode->findFirst("title");
			if (node == NULL) 	
				throw EXmlMissingNode("<stream>", "title");
			p_title.setFromXmlNode(node);
			
			// get description
			node = dtlNode->findFirst("description");
			if (node != NULL)
				p_description.setFromXmlNode(node);
			
			// get license
			node = dtlNode->findFirst("license");
			if (node != NULL)
				p_license.setFromXmlNode(node);
			
			p_metaRead = true;
		}
		
		if (readFlag == rfReadAll || readFlag == rfReadData) {
			// pairs
			sxml::XmlNode* entriesNode = dtlNode->findFirst("entries");
			if (entriesNode == NULL) 
				throw EXmlMissingNode("<stream>", "entries");
			sxml::NodeSearch* ns = entriesNode->findInit("e");
			for (node = entriesNode->findNext(ns); node != NULL; node = entriesNode->findNext(ns)) {
				WordClass::Type wc = WordClass::Undefined;
				Entry* orig = NULL;
				Entry* trans = NULL;
				TrainingEntry* te = NULL;
				
				// word class
				ait = node->attributes.find("t");
				if (ait != node->attributes.end()) {
					wc = WordClass::getType(ait->second);
				}
				
				try {
					sxml::XmlNode* tmpNode;
					
					// original
					tmpNode = node->findFirst("o");
					if (tmpNode == NULL) throw EXmlMissingNode("<stream>", "o");
					
					if (tmpNode->children.size() != 1 ||
						(tmpNode->children.size() == 1 && tmpNode->children[0]->type != sxml::ntTextNode))
					{
						throw EXmlTreeInvalid("", tmpNode->name);
					}
					
					orig = new Entry(p_langProfile_o, tmpNode->children[0]->name, wc);
					
					// translation
					tmpNode = node->findFirst("t");
					if (tmpNode == NULL) throw EXmlMissingNode("<stream>", "t");
					
					if (tmpNode->children.size() != 1 ||
						(tmpNode->children.size() == 1 && tmpNode->children[0]->type != sxml::ntTextNode))
					{
						throw EXmlTreeInvalid("", tmpNode->name);
					}
					
					trans = new Entry(p_langProfile_t, tmpNode->children[0]->name, wc);
					
					te = new TrainingEntry(orig, trans);
					
					// meaning
					tmpNode = node->findFirst("m");
					if (tmpNode != NULL) {
						te->meaning().setFromXmlNode(tmpNode);
					}
					
					// extended info for originating language
					tmpNode = node->findFirst("oe");
					if (tmpNode != NULL)
						te->orig()->setFromXmlNode(tmpNode);
					
					// extended info for translation
					tmpNode = node->findFirst("te");
					if (tmpNode != NULL)
						te->trans()->setFromXmlNode(tmpNode);
					
					p_entries.push_back(te);
					
				} catch (Exception e) {
					if (te != NULL)  
						delete te;
					else {
						if (orig != NULL) delete orig;
						if (trans != NULL) delete trans;
					}
					entriesNode->findFree(ns);
					throw e;
				}
			}
			entriesNode->findFree(ns);
			
			p_dataRead = true;
		}
		
	} catch (EXmlTreeInvalid e) {
		DEBUG(DBG_GENERAL, "TrainingLesson::readFromStream(): Exception occurred: %s\n", e.msg.c_str());
		delete xmlNode;
		throw;
		
	} catch (Exception e) {
		DEBUG(DBG_GENERAL, "TrainingLesson::readFromStream(): Exception occurred: %s\n", e.msg.c_str());
		delete xmlNode;
		throw;
	}
	
	delete xmlNode;
}

void Lesson::readFromFile(const std::string& fileName, const ReadFlag readFlag)
{
	p_fileName = fileName;
	
	// read in lesson file completely 
	ifstream fin(fileName.c_str());
		
	if (fin.fail()) {
		DEBUG(DBG_GENERAL, "TrainingLesson::readFromFile(): could not open file %s\n", 
			fileName.c_str());
		throw EFileError(fileName);
	}
	
	try {
		readFromStream(fin, readFlag);
		fin.close();
		
	} catch (EFileException e) {
		e.fileName = fileName;
		fin.close();
		throw e;
		
	} catch (...) {
		fin.close();
		throw;
		
	}
}

void Lesson::readData() 
{
	if (p_fileName == "") throw EFileError(p_fileName);
	readFromFile(p_fileName, rfReadData);
}

void Lesson::unloadData()
{
	vector<TrainingEntry*>::iterator it;
	for (it = p_entries.begin(); it != p_entries.end(); it++) {
		if (*it != NULL) delete ((TrainingEntry*) *it);
	}
	p_entries.clear();
	p_dataRead = false;
}

void Lesson::write()
{
	if (p_fileName == "") throw EFileError(p_fileName);
	writeToFile(p_fileName);
}

void Lesson::writeToStream(std::ostream& writeTo)
{
	if (writeTo.fail() || writeTo.bad()) throw EFileError("<stream>");
	
	if (p_langProfile_o == NULL) throw EMissingAttribute("langCode_o");
	if (p_langProfile_t == NULL) throw EMissingAttribute("langCode_t");
	if (p_version.empty()) p_version = DVT_DTL_XML_FORMATVERSION;
	
	if (p_title.empty()) throw EMissingElement("title");
	
	sxml::XmlNode* xmlNode;
	sxml::XmlNode* dtlNode;
	sxml::XmlNode* entriesNode;
	sxml::XmlNode* entryNode;
	sxml::XmlNode* node;
	
	// prologue
	
	xmlNode = new sxml::XmlNode(sxml::ntDocumentNode, "xml");
	xmlNode->attributes["version"] = "1.0";
	xmlNode->attributes["encoding"] = "utf-8";
	
	dtlNode = new sxml::XmlNode(sxml::ntElementNode, "divotra:trainingLesson");
	dtlNode->attributes["xmlns:divotra"] = DVT_XML_NAMESPACE;
	dtlNode->attributes["xmlns:xsi"] = "http://www.w3.org/2001/XMLSchema-instance";
	dtlNode->attributes["xsi:schemaLocation"] = DVT_XSD_LOCATION;
	dtlNode->attributes["formatVersion"] = DVT_DTL_XML_FORMATVERSION;
	dtlNode->attributes["langCode_o"] = p_langProfile_o->langCode();
	dtlNode->attributes["langCode_t"] = p_langProfile_t->langCode();
	dtlNode->attributes["version"] = p_version;
		
	xmlNode->children.push_back(dtlNode);	
	
	// meta data
	
	node = new sxml::XmlNode(sxml::ntElementNode, "title");
	p_title.setToXmlNode(node);
	dtlNode->children.push_back(node);
	
	if (!p_description.empty()) {
		node = new sxml::XmlNode(sxml::ntElementNode, "description");
		p_description.setToXmlNode(node);
		dtlNode->children.push_back(node);
	}
	
	if (!p_license.empty()) {
		node = new sxml::XmlNode(sxml::ntElementNode, "license");
		p_license.setToXmlNode(node);
		dtlNode->children.push_back(node);
	}
	
	// data
	
	entriesNode = new sxml::XmlNode(sxml::ntElementNode, "entries");
	dtlNode->children.push_back(entriesNode);
	
	try {
		vector<TrainingEntry*>::iterator entries_it;
		for (entries_it = p_entries.begin(); entries_it != p_entries.end(); entries_it++) {
			TrainingEntry* te = *entries_it;
			assert(te != NULL);
			
			// entry
			entryNode = new sxml::XmlNode(sxml::ntElementNode, "e");
			entriesNode->children.push_back(entryNode);
			if (te->wordClass() != WordClass::Undefined) {
				assert(te->wordClass() < WordClass::High);
				entryNode->attributes["t"] = WordClass::getXmlAttr(te->wordClass());
			}
			
			// original
			if (te->orig() == NULL || te->orig()->baseName().empty()) 
				throw EMissingElement("original");
			node = new sxml::XmlNode(sxml::ntElementNode, "o");
			entryNode->children.push_back(node);
			node->children.push_back(new sxml::XmlNode(sxml::ntTextNode, te->orig()->baseName()));
			
			// translation
			if (te->trans() == NULL || te->trans()->baseName().empty()) 
				throw EMissingElement("translation");
			node = new sxml::XmlNode(sxml::ntElementNode, "t");
			entryNode->children.push_back(node);
			node->children.push_back(new sxml::XmlNode(sxml::ntTextNode, te->trans()->baseName()));
			
			// meaning
			if (!te->meaning().empty()) {
				node = new sxml::XmlNode(sxml::ntElementNode, "m");
				te->meaning().setToXmlNode(node);
				entryNode->children.push_back(node);
			}
			
			// extended info for origin
			node = new sxml::XmlNode(sxml::ntElementNode, "oe");
			te->orig()->setToXmlNode(node);
			if (node->children.size() > 0) {
				entryNode->children.push_back(node);
			} else {
				delete node;
			}
			
			// extended info for translation
			node = new sxml::XmlNode(sxml::ntElementNode, "te");
			te->trans()->setToXmlNode(node);
			if (node->children.size() > 0) {
				entryNode->children.push_back(node);
			} else {
				delete node;
			}
			
		}
		
	} catch (Exception e) {
		delete xmlNode;
		throw e;
	}
	
	try {
		xmlNode->writeToStream(writeTo, core->prettyXmlOutput());
		
	} catch (sxml::Exception e) {
		delete xmlNode;
		if (e != sxml::eBadStream) {
			DEBUG(DBG_GENERAL, "Dvt::Lesson::writeToStream(): XML output error\n");
		}
		
		throw EFileWriteError("<stream>");
	}
	delete xmlNode;
}

void Lesson::writeToFile(const std::string& fileName)
{
	p_fileName = fileName;
	
	string backUpName = fileName + ".bak";
	ifstream fin(fileName.c_str());
	if (!fin.fail()) {
		ofstream bakout(backUpName.c_str());
		if (bakout.fail()) {
			DEBUG(DBG_GENERAL, "Dvt::Lesson::writeToFile(): could not open file %s\n", 
				backUpName.c_str());
			throw EFileError(backUpName);
		}
		
		char c;
		while (fin.get(c)) bakout << c;
		bakout.close();
		
	}
	
	ofstream fout(fileName.c_str());
	
	if (fout.fail()) {
		DEBUG(DBG_GENERAL, "Dvt::Lesson::writeToFile(): could not open file %s\n", 
			fileName.c_str());
		throw EFileError(fileName);
	}
	
	try {
		writeToStream(fout);
		fout.close();
		
	} catch (EFileException e) {
		e.fileName = fileName;
		fout.close();
		throw e;
		
	} catch (...) {
		fout.close();
		throw;
		
	}
}


} // namespace

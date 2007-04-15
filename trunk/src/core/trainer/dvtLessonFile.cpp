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

#include "dvtLessonFile.h"

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

LessonFile::LessonFile(bool newLesson)
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

LessonFile::~LessonFile()
{
	unloadData();
}

LanguageProfile* LessonFile::langProfile_o() {return p_langProfile_o;}
LanguageProfile* LessonFile::langProfile_t() {return p_langProfile_t;}
std::string LessonFile::version() {return p_version;}

void LessonFile::setLangProfile_o(LanguageProfile* lp) {p_langProfile_o = lp;}
void LessonFile::setLangProfile_t(LanguageProfile* lp) {p_langProfile_t = lp;}

bool LessonFile::metaRead() {return p_metaRead;}
bool LessonFile::dataRead() {return p_dataRead;}

MlString& LessonFile::title() {return p_title;}
MlString& LessonFile::description() {return p_description;}
MlString& LessonFile::license() {return p_license;}

map<int, Lesson*>& LessonFile::lessons() {return p_lessons;}

void LessonFile::readFromStream(std::istream& readFrom, const ReadFlag readFlag)
{
	sxml::XmlNode* xmlNode = new sxml::XmlNode();
	
	try {
		xmlNode->readFromStream(readFrom, true);
		
	} catch (sxml::Exception e) {
		delete xmlNode;
		DEBUG(DBG_GENERAL, "LessonFile::readFromStream(): eXmlParsingError %i\n", e);
		
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
			sxml::XmlNode* lessonsNode = dtlNode->findFirst("lessons");
			if (lessonsNode == NULL) 
				throw EXmlMissingNode("<stream>", "lessons");
			sxml::NodeSearch* ns = lessonsNode->findInit("lesson");
			for (node = lessonsNode->findNext(ns); node != NULL; node = lessonsNode->findNext(ns)) {
				int number = p_lessons.size()+1;
				sxml::NodeAttributesIterator ait = node->attributes.find("n");
				if (ait != node->attributes.end()) 
					number = atoi(ait->second.c_str());
					
				p_lessons[number] = new Lesson(number, this);
				p_lessons[number]->setFromXmlNode(node);
				
			}
			lessonsNode->findFree(ns);
			
			p_dataRead = true;
		}
		
	} catch (EXmlTreeInvalid e) {
		DEBUG(DBG_GENERAL, "LessonFile::readFromStream(): Exception occurred: %s\n", e.msg.c_str());
		delete xmlNode;
		throw;
		
	} catch (Exception e) {
		DEBUG(DBG_GENERAL, "LessonFile::readFromStream(): Exception occurred: %s\n", e.msg.c_str());
		delete xmlNode;
		throw;
	}
	
	delete xmlNode;
}

void LessonFile::readFromFile(const std::string& fileName, const ReadFlag readFlag)
{
	p_fileName = fileName;
	
	// read in lesson file completely 
	ifstream fin(fileName.c_str());
		
	if (fin.fail()) {
		DEBUG(DBG_GENERAL, "LessonFile::readFromFile(): could not open file %s\n", 
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

void LessonFile::readData() 
{
	if (p_fileName == "") throw EFileError(p_fileName);
	readFromFile(p_fileName, rfReadData);
}

void LessonFile::unloadData()
{
	map<int, Lesson*>::iterator it;
	for (it = p_lessons.begin(); it != p_lessons.end(); it++) {
		if (it->second != NULL)
			delete it->second;
	}
	p_lessons.clear();
	p_dataRead = false;
}

void LessonFile::write()
{
	if (p_fileName == "") throw EFileError(p_fileName);
	writeToFile(p_fileName);
}

void LessonFile::writeToStream(std::ostream& writeTo)
{
	if (writeTo.fail() || writeTo.bad()) throw EFileError("<stream>");
	
	if (p_langProfile_o == NULL) throw EMissingAttribute("langCode_o");
	if (p_langProfile_t == NULL) throw EMissingAttribute("langCode_t");
	if (p_version.empty()) p_version = DVT_DTL_XML_FORMATVERSION;
	
	if (p_title.empty()) throw EMissingElement("title");
	
	sxml::XmlNode* xmlNode;
	sxml::XmlNode* dtlNode;
	sxml::XmlNode* lessonsNode;
	sxml::XmlNode* lessonNode;
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
	
	lessonsNode = new sxml::XmlNode(sxml::ntElementNode, "lessons");
	dtlNode->children.push_back(lessonsNode);
	
	try {
		char buf[12];
		map<int, Lesson*>::iterator lessons_it;
		for (lessons_it = p_lessons.begin(); lessons_it != p_lessons.end(); lessons_it++) {
			Lesson* lesson = lessons_it->second;
			lessonNode = new sxml::XmlNode(sxml::ntElementNode, "lesson");
			snprintf(buf, 12, "%d", lesson->number());
			lessonNode->attributes["n"] = buf;
			lesson->setToXmlNode(lessonNode);
			lessonsNode->children.push_back(lessonNode);
			
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
			DEBUG(DBG_GENERAL, "Dvt::LessonFile::writeToStream(): XML output error\n");
		}
		
		throw EFileWriteError("<stream>");
	}
	delete xmlNode;
}

void LessonFile::writeToFile(const std::string& fileName)
{
	p_fileName = fileName;
	
	string backUpName = fileName + ".bak";
	ifstream fin(fileName.c_str());
	if (!fin.fail()) {
		ofstream bakout(backUpName.c_str());
		if (bakout.fail()) {
			DEBUG(DBG_GENERAL, "Dvt::LessonFile::writeToFile(): could not open file %s\n", 
				backUpName.c_str());
			throw EFileError(backUpName);
		}
		
		char c;
		while (fin.get(c)) bakout << c;
		bakout.close();
		
	}
	
	ofstream fout(fileName.c_str());
	
	if (fout.fail()) {
		DEBUG(DBG_GENERAL, "Dvt::LessonFile::writeToFile(): could not open file %s\n", 
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

void LessonFile::createLesson(int pos, const MlString& title)
{
	if (p_lessons[pos] != NULL) {
		DEBUG(DBG_GENERAL, "Dvt::LessonFile::createLesson(): WARNING: overwriting existing lesson");
		delete p_lessons[pos];
		
	}
	
	Lesson* lesson = new Lesson(pos, this);
	lesson->setTitle(title);
	p_lessons[pos] = lesson;
}

void LessonFile::deleteLesson(Lesson* lesson)
{
	p_lessons.erase(p_lessons.find(lesson->number()));
	delete lesson;
}

} // namespace Dvt

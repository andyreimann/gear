// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "LocationBindingsBlockParser.h"
#include "LocationBinding.h"
#include "StringTools.h"
#include "Logger.h"
#include "FileResource.h"

#include <sstream>
#include <algorithm>



using namespace G2;

LocationBindingsBlockParser::LocationBindingsBlockParser(FileResource* file) 
    : mFile(file)
{
}

void
LocationBindingsBlockParser::parse() 
{
    std::stringstream log;

    if(mFile == nullptr)
    {
        
        logger << "[LocationBindingsBlockParser] -> Error 1001: given filehandle is 0\n";
        return;
    }
    logger << "[LocationBindingsBlockParser] -> start parsing LocationBindings block\n";

    
    int curvedBracketsOpened = 0;

    if(mFile->isOpen()) 
    {
        while(!mFile->eof()) 
        {
            log.str("");
            std::string line = mFile->getLine();

            // check if we have reached the end of the block
            curvedBracketsOpened += (int)std::count(line.begin(), line.end(), '{');
            curvedBracketsOpened -= (int)std::count(line.begin(), line.end(), '}');

            if(curvedBracketsOpened <= -1) 
            {
                // block content parsed
                logger << "[LocationBindingsBlockParser] -> done parsing LocationBindings block\n";
                return;
            }

            std::stringstream lineStr;
            lineStr << line;

            std::string name, niceName, datatype;
            std::string semantic;
            lineStr >> name;
            if(name.substr(0,2) == "//")
            {
                continue;
            }
            
            // parse nice name 
            size_t startPos = line.find_first_of("\"");
            size_t endPos = line.find_last_of("\"");
            if(startPos != std::string::npos && endPos != std::string::npos) 
            {
                niceName = line.substr(startPos+1, endPos-startPos-1);
            }
            // parse datatype
            startPos = line.find_first_of(",");
            endPos = line.find_last_of(")");
            if(startPos != std::string::npos && endPos != std::string::npos) 
            {
                datatype = Tools::String::trim(line.substr(startPos+1, endPos-startPos-1));
            }
            // parse location
            startPos = line.find_first_of("{");
            endPos = line.find_last_of("}");
            if(startPos != std::string::npos && endPos != std::string::npos) 
            {
                semantic = Tools::String::trim(line.substr(startPos+1, endPos-startPos-1));
            }

            Semantics::Name parsedSemantic = Semantics::getSemantic(semantic);

            if(parsedSemantic == Semantics::SEMANTIC_INVALID)
            {

                logger << "[LocationBindingsBlockParser] -> Warning: LocationBinding '" << name << "' has no valid semantic value given! Valid values are: POSITION,BLENDWEIGHT,NORMAL,COLOR_0,COLOR_1,FOGCOORD,PSIZE,BLENDINDICES,TANGENT,BINORMAL,TEXCOORD_0-TEXCOORD_7\n";
                continue; // ignore this location binding
            }

            logger << "[LocationBindingsBlockParser] -> Add LocationBinding[semantic=" << semantic << ";name=" << name << ";nname=" << niceName << ";dt=" << datatype << "]\n";

            mLocationBindings.push_back(LocationBinding(parsedSemantic, name, niceName, datatype));
        }
    }
    logger << "[LocationBindingsBlockParser] -> done parsing LocationBindings block\n";
}
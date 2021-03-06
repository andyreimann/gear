// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "UberShaderParser.h"
#include "FileResource.h"
#include "UberShaderBlockParser.h"

#include <sstream>
#include <iostream>


using namespace G2;

enum ParserState {
    AWAIT_BLOCK = 1,
    READ_BLOCK_CONTENT,
};



std::shared_ptr<Effect::Builder>
UberShaderParser::parse(std::string const& fileName) 
{
    std::shared_ptr<Effect::Builder> builder = std::shared_ptr<Effect::Builder>(new Effect::Builder);

    ParserState currentState = AWAIT_BLOCK;

    std::stringstream log;

    FileResource file(fileName);

    if(!file.isOpen()) {

        log << "[UberShaderParser] -> could not open file '" << fileName << "'" << std::endl;
        //qDebug(log.str().c_str());
        return builder;
    }

    while(!file.eof()) 
    {
        log.str("");
        std::string line = file.getLine();
        std::stringstream lineStr;
        lineStr << line;

        switch(currentState) 
        {
            case AWAIT_BLOCK:
            {
                std::string blockName,uberShaderName;
                lineStr >> blockName;
                if(blockName.substr(0,2) == "//")
                {
                    continue;
                }

                // parse shader name 
                size_t startPos = line.find_first_of("\"");
                size_t endPos = line.find_last_of("\"");
                if(startPos != std::string::npos && endPos != std::string::npos) 
                {
                    uberShaderName = line.substr(startPos+1, endPos-startPos-1);
                }

                currentState = READ_BLOCK_CONTENT;
                UberShaderBlockParser blockParser(builder.get(), &file);
                blockParser.parse();
                break;
            }
            default:
                break;
        }
    }

    builder->buildAndCompile();
    return builder;
}
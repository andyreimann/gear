// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "FileResource.h"
#include "Logger.h"

using namespace G2;

FileResource::FileResource(std::string const& name) 
	: mLoaded(false),
	mName(name),
	mFileHandle(mName)
{
}

FileResource::FileResource(FileResource const& rhs) 
	: mLoaded(rhs.mLoaded),
	mName(rhs.mName),
	mContent(rhs.mContent),
	mFileHandle(rhs.mName)
{ }

FileResource&
FileResource::operator=(FileResource const& rhs) 
{
	// do assignment here
	mLoaded = rhs.mLoaded;
	mName = rhs.mName;
	mContent = rhs.mContent;
	mFileHandle = std::ifstream(mName);
	return *this;
}

FileResource::~FileResource() 
{
	if(mFileHandle.is_open()) 
	{
		mFileHandle.close();
	}
}

bool
FileResource::load() 
{
	// open file
	if(!mLoaded)
	{
		std::ifstream fileHandle(mName);

		if(fileHandle.is_open()) 
		{

			std::string line;
			while(fileHandle.good())
			{
		
				// fetch line
				std::stringstream lineStream;
				getline( fileHandle, line );
				lineStream << line;

				// parse line
				//++mNumLines_;
				mContent.append(lineStream.str()+"\n");
			}
			mLoaded = true;
			return true;
		}
	}
	return false;
}

bool
FileResource::isOpen() const 
{
	return mFileHandle.is_open();
}

bool
FileResource::eof() const 
{
	return !mFileHandle.good();
}

std::string
FileResource::getLine() 
{
	std::string line;
	getline( mFileHandle, line );
	return line;
}
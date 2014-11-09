#include "RootImporter.h"
#include "Logger.h"

void
G2::RootImporter::beginBlock()
{
	logger << "[RootImporter] Begin " << endl;
}

void
G2::RootImporter::processLine(std::string const& line)
{
	logger << "[RootImporter] Import line " << line << endl;
}

void
G2::RootImporter::endBlock()
{
	logger << "[RootImporter] End " << endl;
}
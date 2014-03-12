#include "Effect.h"

using namespace G2;

Effect::Effect() 
{
}

Effect::Effect(Effect const& rhs) 
{
	// do copy here
}

Effect&
Effect::operator=(Effect const& rhs) 
{
	// do assignment here
	return *this;
}

Effect::~Effect() 
{
}

std::shared_ptr<Effect>
Effect::Builder::buildResource() 
{

}
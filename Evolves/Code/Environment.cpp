#include "Environment.h"
#include "EnvironAsset.h"

Environment::Environment( Device *myDevice )
{
	//Spawn 100 stars
	for(int i=0; i < 100; i++)
	{
		EnvironAsset *star = new EnvironAsset( myDevice );
		stars->push_back(star);
	}
}


Environment::~Environment()
{

}

HRESULT Environment::Draw()
{
	HRESULT hr = S_OK;

	std::vector<EnvironAsset *>::iterator it;

	for ( it=stars->begin() ; it != stars->end(); it++ )
	{
		hr = (*it)->Draw();
		if( FAILED(hr) )
		{
			return hr;
		}
	}
	
	return S_OK;
}
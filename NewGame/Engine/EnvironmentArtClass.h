//=================================
// include guard
#ifndef EnvironmentArtClass_H_
#define EnvironmentArtClass_H_

//=================================
// forward declared dependencies
class StarClass;

//=================================
// included dependencies
#include <vector>

class EnvironmentArtClass{

public:
	EnvironmentArtClass();
	~EnvironmentArtClass();
	
	bool Initialize();
	bool RenderAll();

private:
	std::vector<StarClass>* allStars;

};

#endif /* EnvironmentArtClass_H_ */
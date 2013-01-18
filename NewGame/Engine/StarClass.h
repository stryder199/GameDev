//=================================
// include guard
#ifndef StarClass_H_
#define StarClass_H_

//=================================
// forward declared dependencies
class MeshClass;

//=================================
// included dependencies

class StarClass{

public:
	StarClass();
	~StarClass();
	
	bool Initialize();
	bool Render();

private:
	MeshClass* mesh;

};

#endif /* StarClass_H_ */
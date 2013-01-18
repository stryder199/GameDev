//=================================
// include guard
#ifndef SoundClass_H_
#define SoundClass_H_

//=================================
// forward declared dependencies


//=================================
// included dependencies

class SoundClass{

public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize();
};

#endif /* SoundClass_H_ */
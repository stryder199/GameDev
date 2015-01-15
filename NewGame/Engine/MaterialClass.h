#pragma once

//////////////
// INCLUDES //
#include <vector>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //

using namespace std;

class MaterialClass
{
public: 
	typedef struct
	{
		float Ns;
		float Ka_r, Ka_g, Ka_b;
		float Kd_r, Kd_g, Kd_b;
		float Ks_r, Ks_g, Ks_b;
		float Ni, d, illum;
		string map_Kd;
	}MaterialInfo;

public:
	MaterialClass();
	~MaterialClass();

    void Initialize(MaterialInfo matInfo);
	void Shutdown();

	MaterialInfo getMaterialInfo();
private:
	MaterialInfo m_materialInfo;
};
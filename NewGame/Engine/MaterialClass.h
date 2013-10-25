//=================================
// include guard
#ifndef MaterialClass_H_
#define MaterialClass_H_

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>

using namespace std;

class MaterialClass
{
public: 
	typedef struct
	{
		float r, g, b, a;
	}ColorType;

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

	bool Initialize(MaterialInfo matInfo);
	void Shutdown();

	vector<ColorType>* getColors();
	int getColorCount();

	void addColorData(ColorType data);

private:
	MaterialInfo m_materialInfo;
	vector<ColorType> m_vecColors;
};

#endif
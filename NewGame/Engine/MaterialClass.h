//=================================
// include guard
#ifndef MaterialClass_H_
#define MaterialClass_H_

//=================================
// forward declared dependencies

//=================================
// included dependencies


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
	}MaterialInfo;

public:
	MaterialClass();
	~MaterialClass();

	bool Initialize(int colorCount);
	void Shutdown();

	ColorType* getColors();
	int getColorCount();

private:
	ColorType* m_Colors;
	int m_colorCount;
};

#endif
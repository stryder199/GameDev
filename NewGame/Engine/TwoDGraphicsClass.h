//=================================
// include guard
#ifndef TwoDGraphicsClass_H_
#define TwoDGraphicsClass_H_

//=================================
// forward declared dependencies
class BitmapClass;
class D3DClass;

//=================================
// included dependencies

class TwoDGraphicsClass{

public:
	TwoDGraphicsClass();
	TwoDGraphicsClass(const TwoDGraphicsClass&);
	~TwoDGraphicsClass();

	bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight);

	bool RenderAll(D3DClass*);

private:
	BitmapClass* m_Bitmap;
};

#endif /* TwoDGraphicsClass_H_ */
//=================================
// include guard
#ifndef Model_H_
#define Model_H_

//=================================
// forward declared dependencies
class Device;
struct ModelData;

//=================================
// included dependencies
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <fstream>

class Model{

private:
	
	int vertexCount, indexCount;
	char *dataFile;
	ModelData *modelData;


public:
	UINT stride;
	UINT offset;
	ID3D11Buffer *g_pVertexBuffer;
	ID3D11Buffer *g_pIndexBuffer;
	ID3D11Buffer *g_pConstantBuffer;


	Model();
	~Model();
	HRESULT init( char *filename, Device *myDevice );
	HRESULT importMeshData();
	HRESULT initBuffers( Device *myDevice );

};

#endif /* Model_H_ */
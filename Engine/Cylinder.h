////////////////////////////////////////////////////////////////////////////////
// Filename: Cylinder.h
////////////////////////////////////////////////////////////////////////////////


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>
#include <list>
#include <math.h>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: CylinderClass
////////////////////////////////////////////////////////////////////////////////
class Cylinder
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR4 color;
	};

	struct VectorType
	{
		float x, y, z;
	};

public:
	Cylinder(ID3D11Device* device, D3DXVECTOR3 buildPos, D3DXVECTOR3 rotationVal, float bottomR, float topR, float height, int slices, int stacks, float scale);
	~Cylinder();
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	D3DXVECTOR3 startPos, endPos;
	D3DXVECTOR3 rotationVal;
	int m_indexCount;
	float bottomR, topR, height;
	bool InitializeBuffers(ID3D11Device * device);

private:
	float slices, stack;
	float pi = 3.1415926535897;
	int m_vertexCount;
	float scale;
	D3DXVECTOR3 buildPos;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext * device);
};


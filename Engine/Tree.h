////////////////////////////////////////////////////////////////////////////////
// Filename: Tree.h
////////////////////////////////////////////////////////////////////////////////


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>
#include "Cylinder.h"
#include <list>
#include <vector>
#include <ctime>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: TreeClass
////////////////////////////////////////////////////////////////////////////////
class Tree
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
	Tree();
	~Tree();
	int GetIndexCount();
	bool InitializeTree(ID3D11Device*);
	void Shutdown();
	bool GenerateTree(ID3D11Device* device, bool keydown, bool keydown2, int iterations);
	bool GenerateTerrainTrees(ID3D11Device * device, D3DXVECTOR3 spawnPos, int iterations, float scale);
	D3DXVECTOR3 GetRotation();
	vector<Cylinder*> branches;

private:
	Cylinder* savedBranch;
	int currentIt;
	int savePoint;
	int savePoint2;
	bool treeToggle;
	string lsystem;
	string nextLsystem;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};


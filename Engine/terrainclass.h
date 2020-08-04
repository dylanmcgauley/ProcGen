////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>
#include "Tree.h"
#include <vector>


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR3 normal;
		D3DXVECTOR4 color;
	};

	struct HeightMapType 
	{ 
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
		int regionID;
	};

	struct VectorType 
	{ 
		float x, y, z;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, char*);
	bool InitializeTerrain(ID3D11Device*, int terrainWidth, int terrainHeight);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	bool GenerateHeightMap(ID3D11Device* device, bool keydown);
	bool GenerateFaulting(ID3D11Device* device, bool keydown);
	bool GenerateVoroni(ID3D11Device* device, bool keydown);
	bool AddTrees(ID3D11Device * device, bool keydown);
	bool RegionSelector(ID3D11Device * device, bool left, bool right);
	void SelectRegionGeneration(bool select);
	bool SelectMapColorMode(ID3D11Device* device, bool select);
	float FindDistance(D3DXVECTOR2 point1, D3DXVECTOR2 point2);
	bool SmoothTerrain(ID3D11Device* device, bool keydown, bool keydown2, bool keydown3);
	bool GenerateRandomHeightField(ID3D11Device* device, bool keydown);
	int  GetIndexCount();

	vector<Tree*> trees;

private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:
	bool m_terrainGeneratedToggle;
	bool regionGeneration;
	bool colorMap;
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	int selectedRegion;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;
};

#endif
#include "Cylinder.h"

Cylinder::Cylinder(ID3D11Device * device, D3DXVECTOR3 buildPos, D3DXVECTOR3 rotationVal, float bottomR, float topR, float height, int slices, int stacks, float scale)
{
	this->bottomR = bottomR;
	this->topR = topR;
	this->height = height;
	this->slices = slices;
	this->stack = stacks;
	this->buildPos = buildPos;
	this->rotationVal = rotationVal;
	this->scale = scale;
	m_vertexCount = 0;
	m_indexCount = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	startPos = D3DXVECTOR3(0, 0, 0);
	endPos = D3DXVECTOR3(0, 0, 0);

	InitializeBuffers(device);
}


Cylinder::~Cylinder()
{
}

void Cylinder::Shutdown()
{
	// Release the vertex and index buffer.
	ShutdownBuffers();

	return;
}

void Cylinder::Render(ID3D11DeviceContext * device)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(device);

	return;
}

bool Cylinder::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Calculate the number of vertices in the tree mesh.
	m_vertexCount = ((slices * stack) * 6);

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// BUILD CYLINDER HERE
	float radiusStep = (topR - bottomR) / stack;
	float stackHeight = height / stack;

	// loop through for the amount of stacks / rings
	for (int i = 0; i < stack + 1; i++)
	{
		// calculate the height radius and angle 
		float currentHeight = (-0.5f * height) + (i * stackHeight);
		float currentRadius = bottomR + (i * radiusStep);
		float theta = 2 * pi / slices;

		// loop of the amount of slices in the stack
		for (int j = 0; j <= slices; j++)
		{
			// set up rotation angles
			float xAngle = cos(j * theta);
			float zAngle = sin(j * theta);
			index = (i * slices) + j;

			// calculate the vertices position
			D3DXVECTOR3 verticePos = D3DXVECTOR3(currentRadius * xAngle, currentHeight, currentRadius * zAngle);

			// set the position of the vertex
			vertices[index].position = D3DXVECTOR3((verticePos.x * scale) + buildPos.x, ((verticePos.y + (height * 0.5f)) * scale) + buildPos.y, (verticePos.z * scale) + buildPos.z);
			vertices[index].normal = D3DXVECTOR3(0, 1, 0);
			vertices[index].color = D3DXVECTOR4(0.2f, 0.15f, 0.03f, 1.0f);
		}
		// offsets the vertices in each stack (in X and Z) to give the illusion of rotation
		buildPos.x += rotationVal.x;
		buildPos.z += rotationVal.z;
	}
	int lastIndex = index;
	int ringVertexCount = slices;
	int index2 = 0;
	for (int i = 0; i < stack; i++) {
		for (int j = 0; j < slices; j++) {
			// set up the cylinders indices
			indices[index2] = (i * ringVertexCount + j);
			index2++;
			indices[index2] = ((i + 1) * ringVertexCount + j);
			index2++;
			indices[index2] = ((i + 1) * ringVertexCount + j + 1);
			index2++;

			indices[index2] = (i * ringVertexCount + j);
			index2++;
			indices[index2] = ((i + 1) * ringVertexCount + j + 1);
			index2++;
			indices[index2] = (i * ringVertexCount + j + 1);
			index2++;
		}
	}
	// set the start and end position of the cylinder for use in the next branch
	startPos = D3DXVECTOR3(vertices[0].position.x, vertices[0].position.y, vertices[0].position.z);
	endPos = D3DXVECTOR3(vertices[lastIndex].position.x, vertices[lastIndex].position.y, vertices[lastIndex].position.z);

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Cylinder::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void Cylinder::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
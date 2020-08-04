#include "Tree.h"

Tree::Tree()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	currentIt = 0;
	lsystem = "F";
	nextLsystem = "";
	savePoint = 0;
	treeToggle = false;
	// get random seed
	srand(time(NULL));
}


Tree::~Tree()
{
}

int Tree::GetIndexCount()
{
	int m_indexCount = 0;

	for (auto &branch : branches)
	{
		m_indexCount += branch->m_indexCount;
	}
	return m_indexCount;
}

bool Tree::InitializeTree(ID3D11Device *)
{
	return false;
}

void Tree::Shutdown()
{
	// shutdown the branches
	for (auto &branch : branches)
	{
		branch->Shutdown();
	}

	return;
}

// iterates through an L Sytem and generates a tree
bool Tree::GenerateTree(ID3D11Device * device, bool keydown, bool keydown2, int iterations)
{
	// set initial rotation value
	D3DXVECTOR3 rotation = D3DXVECTOR3(0, 0, 0);
	if ((keydown2 || keydown) && !treeToggle)
	{
		string rule;

		// chose rule depending on which button was pressed
		if (keydown)
		{
			rule = "S+FU+FR-FB-FU+FR-FB+FU+FR-FB-FU+FR-FBF";
		}
		else
		{
			rule = "FF+S+F-F-FB-S-F+F+FB";
		}

		// loop for the desired iterations
		for (int i = 0; i < iterations; i++)
		{
			for (char& x : lsystem)
			{
				// creates a cylinder
				if (x == 'F')
				{
					// initializes the first branch
					if (branches.size() <= 0)
					{
						branches.push_back(new Cylinder(device, D3DXVECTOR3(50, 10, 50), rotation, 2, 2, 20, 20, 20, 1));
						savedBranch = branches.at(branches.size() - 1);
					}
					else
					{
						branches.push_back(new Cylinder(device, D3DXVECTOR3(savedBranch->endPos.x - (savedBranch->topR), savedBranch->endPos.y, savedBranch->endPos.z), rotation, savedBranch->topR, savedBranch->topR / 1.2, savedBranch->height / 1.1, 20, 20, 1));
					}
					// create the next line for the L System
					// L Sysyem Rule: F -> S+FU+FR-FB-FU+FR-FB+FU+FR-FB-FU+FR-FBF
					nextLsystem += rule;
				}
				// rotate the branch in positive directions (X & Z)
				if (x == '+')
				{
					rotation += GetRotation();
				}
				// rotate the branch in negative directions (X & Z)
				else if (x == '-')
				{
					rotation -= GetRotation();
				}
				// push list to save branch position
				else if (x == 'S')
				{
					savePoint = branches.size() - 1;
				}
				else
				{
					rotation = D3DXVECTOR3(0, 0, 0);
				}
				// pop list to go back to saved position
				if (x == 'B')
				{
					savedBranch = branches.at(savePoint);
				}
				if (x == 'U')
				{
					savePoint2 = branches.size() - 1;
				}
				if (x == 'R')
				{
					savedBranch = branches.at(savePoint2);
				}
			}
			// set the l system to the newly created one
			savedBranch = branches.at(branches.size() - 1);
			rotation = D3DXVECTOR3(0, 0, 0);
			lsystem = nextLsystem;
		}
		treeToggle = true;
	}
	else
	{
		treeToggle = false;
	}

	return false;
}

bool Tree::GenerateTerrainTrees(ID3D11Device * device, D3DXVECTOR3 spawnPos, int iterations, float scale)
{
	// set initial rotation value
	D3DXVECTOR3 rotation = D3DXVECTOR3(0, 0, 0);
	string rule;

		rule = "S+FU+FR-FB-FU+FR-FB+FU+FR-FB-FU+FR-FBF";

		// loop for the desired iterations
		for (int i = 0; i < iterations; i++)
		{
			for (char& x : lsystem)
			{
				// creates a cylinder
				if (x == 'F')
				{
					// initializes the first branch
					if (branches.size() <= 0)
					{
						branches.push_back(new Cylinder(device, spawnPos, rotation * scale, 2, 2, 20, 10, 10, scale));
						savedBranch = branches.at(branches.size() - 1);
					}
					else
					{
						branches.push_back(new Cylinder(device, D3DXVECTOR3(savedBranch->endPos.x - (savedBranch->topR * scale), savedBranch->endPos.y, savedBranch->endPos.z), rotation * scale, savedBranch->topR, savedBranch->topR / 1.2, savedBranch->height / 1.1, 10, 10, scale));
					}
					// create the next line for the L System
					// L Sysyem Rule: F -> S+FU+FR-FB-FU+FR-FB+FU+FR-FB-FU+FR-FBF
					nextLsystem += rule;
				}
				// rotate the branch in positive directions (X & Z)
				if (x == '+')
				{
					rotation += GetRotation();
				}
				// rotate the branch in negative directions (X & Z)
				else if (x == '-')
				{
					rotation -= GetRotation();
				}
				// push list to save branch position
				else if (x == 'S')
				{
					savePoint = branches.size() - 1;
				}
				else
				{
					rotation = D3DXVECTOR3(0, 0, 0);
				}
				// pop list to go back to saved position
				if (x == 'B')
				{
					savedBranch = branches.at(savePoint);
				}
				if (x == 'U')
				{
					savePoint2 = branches.size() - 1;
				}
				if (x == 'R')
				{
					savedBranch = branches.at(savePoint2);
				}
			}
			// set the l system to the newly created one
			savedBranch = branches.at(branches.size() - 1);
			rotation = D3DXVECTOR3(0, 0, 0);
			lsystem = nextLsystem;
		}

	return false;
}

// returns a random Vector3 for the branches rotation
D3DXVECTOR3 Tree::GetRotation()
{
	float randX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.8));
	float randZ = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.8));

	return D3DXVECTOR3(randX, 0, randZ);
}
#pragma once

#include "cMat.h"
#include "cVec.h"
#include "Array2d.h"

template<class T>
class cGrid
{
	Vec2 gridStart;
	Vec2 gridSize;
	Vec2 nodeSize;
	IntVec2 nodeCount;

	Array2d<std::vector<T*>> data;

	int arrayIndexForNodeIndex(const IntVec2& nodeIndex)
	{
		return nodeIndex.y * nodeCount.x + nodeIndex.x;
	}

public:
	cGrid() 
	{
	}

	~cGrid() 
	{
	}

	void init(const Vec2& gridStart, const Vec2& gridSize, const Vec2& nodeSize)
	{
		this->gridStart = gridStart;
		this->nodeSize = nodeSize;

		nodeCount.x = (int)ceil(gridSize.x / nodeSize.x);
		nodeCount.y = (int)ceil(gridSize.y / nodeSize.y);

		this->gridSize.x = nodeCount.x * nodeSize.x;
		this->gridSize.y = nodeCount.y * nodeSize.y;

		data.init(nodeCount);
	}

	IntVec2 getNodeIndex(const Vec2& pos) const
	{
		Vec2 diff = pos - gridStart;
		return IntVec2((int)floor(diff.x / nodeSize.x), (int)floor(diff.y / nodeSize.y));
	}

	void insertToGrid(T* object)
	{
		Vec2 objectPos = object->getPosition();
		float radius = object->getRadius();
		object->gridStart = getNodeIndex(objectPos - radius);
		object->gridEnd = getNodeIndex(objectPos + radius);

		for (int x = object->gridStart.x; x <= object->gridEnd.x; x++)
		{
			for (int y = object->gridStart.y; y <= object->gridEnd.y; y++)
			{
				data[x][y].push_back(object);
			}
		}
	}

	void removeFromGrid(T* object)
	{
		for (int x = object->gridStart.x; x <= object->gridEnd.x; x++)
		{
			for (int y = object->gridStart.y; y <= object->gridEnd.y; y++)
			{
				auto& v = data[x][y];
				v.erase(std::remove(v.begin(), v.end(), object), v.end());
			}
		}
	}

	void relocate(T* object)
	{
		Vec2 objectPos = object->getPosition();
		float radius = object->getRadius();
		IntVec2 gridStart = getNodeIndex(objectPos - radius);
		IntVec2 gridEnd = getNodeIndex(objectPos + radius);
		if (object->gridStart != gridStart || object->gridEnd != gridEnd)
		{
			removeFromGrid(object);
			insertToGrid(object);
		}
	}

	void drawDebug()
	{
		for (int x = 0; x < nodeCount.y; x++)
		{
			debugRenderer.addLine(
				Vec2(gridStart.x, gridStart.y + x * nodeSize.y),
				Vec2(gridStart.x + gridSize.w, gridStart.y + x * nodeSize.y),
				0.0f, Vec4::fromColor(0x550000FF));
		}
		for (int y = 0; y < nodeCount.x; y++)
		{
			debugRenderer.addLine(
				Vec2(gridStart.x + y * nodeSize.x, gridStart.y),
				Vec2(gridStart.x + y * nodeSize.x, gridStart.y + gridSize.h),
				0.0f, Vec4::fromColor(0x550000FF));
		}


		for (int x = 0; x < nodeCount.x -1; x++)
		{
			for (int y = 0; y < nodeCount.y - 1; y++)
			{
				auto& v = data[x][y];

				if (v.size())
				{
					debugRenderer.addLine(gridStart + Vec2((float)x, (float)y) * nodeSize, gridStart + Vec2((float)x + 1, (float)y) * nodeSize, 0.0f, Vec4::fromColor(0xFFFF0000));
					debugRenderer.addLine(gridStart + Vec2((float)x, (float)y) * nodeSize, gridStart + Vec2((float)x, (float)y + 1) * nodeSize, 0.0f, Vec4::fromColor(0xFFFF0000));
					debugRenderer.addLine(gridStart + Vec2((float)x + 1, (float)y + 1) * nodeSize, gridStart + Vec2((float)x + 1, (float)y) * nodeSize, 0.0f, Vec4::fromColor(0xFFFF0000));
					debugRenderer.addLine(gridStart + Vec2((float)x + 1, (float)y + 1) * nodeSize, gridStart + Vec2((float)x, (float)y + 1) * nodeSize, 0.0f, Vec4::fromColor(0xFFFF0000));
				
					for (auto& o : v)
					{
						if (o->gridEnd == IntVec2(x, y))
						{
							debugRenderer.addCircle(o->getPosition(), o->getRadius(), 0.0f, Vec4::fromColor(0xFFFF0000));
						}
					}
				}
			}
		}
	}


	std::vector<T*> getNodeAtPos(const Vec2& pos) const
	{
		return data[getNodeIndex(pos)];
	}

	const IntVec2& getNodeCount() const
	{
		return nodeCount;
	}


	const std::vector<T*> getNodeAtIndex(int i, int j) const
	{
		return data[i][j];
	}

	const std::vector<T*> getNodeAtIndex(const IntVec2& pos) const
	{
		return data[i];
	}

};
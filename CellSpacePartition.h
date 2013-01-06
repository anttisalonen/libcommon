#ifndef COMMON_CELLSPACEPARTITION_H
#define COMMON_CELLSPACEPARTITION_H

#include <list>
#include <vector>
#include <cmath>
#include <cassert>

#include "Math.h"
#include "Vector2.h"
#include "Partition.h"

namespace Common {

template<class T>
struct Cell {
	std::list<T> members;
	AABB box;
	Cell(const AABB& b) : box(b) { }
};

template<class T>
class CellSpacePartition {
	public:
		// maxentities is the maximum number of T's returned by any single query
		inline CellSpacePartition(float w, float h,
				unsigned int cellsx, unsigned int cellsy, unsigned int maxentities);
		inline void add(T& t, const Vector2& p);
		inline void remove(T& t, const Vector2& p);
		inline void update(T& t, const Vector2& oldpos, const Vector2& newpos);
		inline T& queryBegin(const Vector2& p, float radius) const;
		inline bool queryEnd() const;
		inline T& queryNext() const;

	private:
		inline unsigned int positionToIndex(const Vector2& p) const;
		std::vector<Cell<T>> mCells;
		float mWidth;
		float mHeight;

		unsigned int mNumCellsX;
		unsigned int mNumCellsY;

		unsigned int mMaxEntities;

		float mCellWidth;
		float mCellHeight;

		mutable typename std::vector<T> mQueryResult;
		mutable unsigned int mQueryIndex;
		mutable unsigned int mQueryResultNum;
};

template<class T>
CellSpacePartition<T>::CellSpacePartition(float w, float h, unsigned int cellsx, unsigned int cellsy,
		unsigned int maxentities)
	: mWidth(w),
	mHeight(h),
	mNumCellsX(cellsx),
	mNumCellsY(cellsy),
	mMaxEntities(maxentities),
	mQueryResultNum(0)
{
	mCellWidth  = ceil(mWidth  / (float)mNumCellsX);
	mCellHeight = ceil(mHeight / (float)mNumCellsY);
	for(int j = 0; j < mNumCellsY; j++) {
		for(int i = 0; i < mNumCellsX; i++) {
			mCells.push_back(Cell<T>(AABB(Vector2((i + 0.5f) * mCellWidth - mWidth * 0.5f,
							(j + 0.5f) * mCellHeight - mHeight * 0.5f),
						Vector2(mCellWidth * 0.5f,
							mCellHeight * 0.5f))));
		}
	}

	std::cout << "Total " << mCells.size() << " cells.\n";

	mQueryResult.resize(mMaxEntities);
}

template<class T>
void CellSpacePartition<T>::add(T& t, const Vector2& p)
{
	auto i = positionToIndex(p);
	mCells.at(i).members.push_back(t);
}

template<class T>
void CellSpacePartition<T>::remove(T& t, const Vector2& p)
{
	auto i = positionToIndex(p);
	mCells.at(i).members.remove(t);
}

template<class T>
void CellSpacePartition<T>::update(T& t, const Vector2& oldpos, const Vector2& newpos)
{
	auto i1 = positionToIndex(oldpos);
	auto i2 = positionToIndex(newpos);
	if(i1 == i2)
		return;

	remove(t, oldpos);
	add(t, newpos);
}

template<class T>
T& CellSpacePartition<T>::queryBegin(const Vector2& p, float radius) const
{
	AABB query(p, Vector2(radius, radius));

	Vector2 minp = Vector2(clamp(mWidth * -0.5f, p.x - radius, mWidth * 0.5f - 0.01f), clamp(mHeight * -0.5f, p.y - radius, mHeight * 0.5f - 0.01f));
	Vector2 maxp = Vector2(clamp(mWidth * -0.5f, p.x + radius, mWidth * 0.5f - 0.01f), clamp(mHeight * -0.5f, p.y + radius, mHeight * 0.5f - 0.01f));
	unsigned int minx = positionToIndex(minp) % int(mNumCellsY);
	unsigned int miny = positionToIndex(minp) / int(mNumCellsY);

	unsigned int maxx = positionToIndex(maxp) % int(mNumCellsY);
	unsigned int maxy = positionToIndex(maxp) / int(mNumCellsY);

	mQueryResultNum = 0;
	for(int j = miny; j <= maxy; j++) {
		for(int i = minx; i <= maxx; i++) {
			auto& c = mCells.at(j * mNumCellsY + i);
			for(auto m : c.members) {
				assert(mQueryResultNum < mQueryResult.size());
				mQueryResult.at(mQueryResultNum++) = m;
			}
		}
	}

	mQueryIndex = 0;

	return mQueryResult[mQueryIndex];
}

template<class T>
bool CellSpacePartition<T>::queryEnd() const
{
	return mQueryIndex >= mQueryResultNum;
}

template<class T>
T& CellSpacePartition<T>::queryNext() const
{
	return mQueryResult.at(++mQueryIndex);
}

template<class T>
unsigned int CellSpacePartition<T>::positionToIndex(const Vector2& p) const
{
	int val = (int(p.y + mHeight * 0.5f) / int(mCellHeight)) * mNumCellsY + (int(p.x + mWidth * 0.5f) / int(mCellWidth));
	return val;
}

}

#endif


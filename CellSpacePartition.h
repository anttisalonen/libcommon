#ifndef COMMON_CELLSPACEPARTITION_H
#define COMMON_CELLSPACEPARTITION_H

#include <list>
#include <vector>

#include "Math.h"

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
		inline CellSpacePartition(float w, float h,
				unsigned int cellsx, unsigned int cellsy, unsigned int maxentities);
		inline void add(T& t, const Point& p);
		inline void remove(T& t, const Point& p);
		inline void update(T& t, const Point& oldpos, const Point& newpos);
		inline T& queryBegin(const Point& p, float radius);
		inline bool queryEnd() const;
		inline T& queryNext();

	private:
		inline unsigned int positionToIndex(const Point& p) const;
		std::vector<Cell<T>> mCells;
		float mWidth;
		float mHeight;

		unsigned int mNumCellsX;
		unsigned int mNumCellsY;

		unsigned int mMaxEntities;

		float mCellWidth;
		float mCellHeight;

		typename std::vector<T> mQueryResult;
		unsigned int mQueryIndex;
		unsigned int mQueryResultNum;
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
	mCellWidth  = mWidth  / mNumCellsX;
	mCellHeight = mHeight / mNumCellsY;
	for(int j = 0; j < mNumCellsY; j++) {
		for(int i = 0; i < mNumCellsX; i++) {
			mCells.push_back(Cell<T>(AABB(Point((i + 0.5f) * mCellWidth - mWidth * 0.5f,
							(j + 0.5f) * mCellHeight - mHeight * 0.5f),
						Point(mCellWidth * 0.5f,
							mCellHeight * 0.5f))));
		}
	}

	std::cout << "Total " << mCells.size() << " cells.\n";

	mQueryResult.resize(mMaxEntities);
}

template<class T>
void CellSpacePartition<T>::add(T& t, const Point& p)
{
	auto i = positionToIndex(p);
	mCells.at(i).members.push_back(t);
}

template<class T>
void CellSpacePartition<T>::remove(T& t, const Point& p)
{
	auto i = positionToIndex(p);
	mCells.at(i).members.remove(t);
}

template<class T>
void CellSpacePartition<T>::update(T& t, const Point& oldpos, const Point& newpos)
{
	auto i1 = positionToIndex(oldpos);
	auto i2 = positionToIndex(newpos);
	if(i1 == i2)
		return;

	remove(t, oldpos);
	add(t, newpos);
}

template<class T>
T& CellSpacePartition<T>::queryBegin(const Point& p, float radius)
{
	AABB query(p, Point(radius, radius));

	Point minp = Point(clamp(mWidth * -0.5f, p.x - radius, mWidth * 0.5f - 0.01f), clamp(mHeight * -0.5f, p.y - radius, mHeight * 0.5f - 0.01f));
	Point maxp = Point(clamp(mWidth * -0.5f, p.x + radius, mWidth * 0.5f - 0.01f), clamp(mHeight * -0.5f, p.y + radius, mHeight * 0.5f - 0.01f));
	unsigned int minx = positionToIndex(minp) % int(mNumCellsY);
	unsigned int miny = positionToIndex(minp) / int(mNumCellsY);

	unsigned int maxx = positionToIndex(maxp) % int(mNumCellsY);
	unsigned int maxy = positionToIndex(maxp) / int(mNumCellsY);

	mQueryResultNum = 0;
	for(int j = miny; j <= maxy; j++) {
		for(int i = minx; i <= maxx; i++) {
			auto& c = mCells.at(j * mNumCellsY + i);
			if(!c.members.empty()) {
				for(auto m : c.members) {
					assert(mQueryResultNum < mQueryResult.size());
					mQueryResult.at(mQueryResultNum++) = m;
				}
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
T& CellSpacePartition<T>::queryNext()
{
	return mQueryResult.at(++mQueryIndex);
}

template<class T>
unsigned int CellSpacePartition<T>::positionToIndex(const Point& p) const
{
	int val = (int(p.y + mHeight * 0.5f) / int(mCellHeight)) * mNumCellsY + (int(p.x + mWidth * 0.5f) / int(mCellWidth));
	return val;
}

}

#endif


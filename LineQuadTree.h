#ifndef COMMON_LINEQUADTREE_H
#define COMMON_LINEQUADTREE_H

#include <cassert>
#include <cmath>

#include <iostream>
#include <map>
#include <queue>
#include <vector>

#include "Partition.h"

namespace Common {

template<class T>
class LineQuadTree;

template<class T>
class LQTIterator {
	public:
		inline LQTIterator(LineQuadTree<T>& qt, bool atend);
		inline bool operator!=(const LQTIterator& other) const;
		inline T operator*();
		inline LQTIterator& operator++();

	private:
		inline LQTIterator& next();

		std::queue<LineQuadTree<T>*> mQTs;
		typename std::map<T, AABB>::iterator mIt;
		bool mEnd = false;
};

template<class T>
class LineQuadTree {
	public:
		inline LineQuadTree(const AABB& boundary);
		inline ~LineQuadTree();
		inline bool insert(T& t, const AABB& p); // invalidates all iterators
		inline bool deleteT(T& t, const AABB& p); // invalidates all iterators
		inline bool update(T& t, const AABB& oldpos, const AABB& newpos); // invalidates all iterators - slow
		inline void clear();
		inline std::vector<T> query(const AABB& area) const;
		inline unsigned int size() const;
		inline LQTIterator<T> begin();
		inline LQTIterator<T> end();

	private:
		inline void subdivide();
		inline bool updateClean(T& t, const AABB& oldpos, const AABB& newpos);
		inline LineQuadTree<T>* find(T& t, const AABB& pos);
		constexpr static const float MIN_DIMENSION = 8.0f;
		AABB mBoundary;
		std::map<T, AABB> mLines;
		LineQuadTree* mNW;
		LineQuadTree* mNE;
		LineQuadTree* mSW;
		LineQuadTree* mSE;

		friend class LQTIterator<T>;
};

template<class T>
LineQuadTree<T>::LineQuadTree(const AABB& boundary)
	: mBoundary(boundary),
	mNW(nullptr),
	mNE(nullptr),
	mSW(nullptr),
	mSE(nullptr)
{
}

template<class T>
LineQuadTree<T>::~LineQuadTree()
{
	if(mNW) {
		delete mNW;
		delete mNE;
		delete mSW;
		delete mSE;
	}
}

template<class T>
bool LineQuadTree<T>::insert(T& t, const AABB& p)
{
	if(!mBoundary.contains(p)) {
		return false;
	}

	if(mBoundary.halfDimension.x * 2.0f <= MIN_DIMENSION ||
			mBoundary.halfDimension.y * 2.0f <= MIN_DIMENSION) {
		mLines.insert({t, p});
		return true;
	}

	if(mNW == nullptr) {
		subdivide();
	}

	if(mNW->insert(t, p)) return true;
	if(mNE->insert(t, p)) return true;
	if(mSW->insert(t, p)) return true;
	if(mSE->insert(t, p)) return true;

	mLines.insert({t, p});
	return true;
}

template<class T>
bool LineQuadTree<T>::deleteT(T& t, const AABB& p)
{
	if(!mBoundary.contains(p)) {
		return false;
	}

	auto it = mLines.find(t);
	if(it != mLines.end()) {
		mLines.erase(it);
		return true;
	}

	if(mNW == nullptr) {
		return false;
	}

	if(mNW->deleteT(t, p)) return true;
	if(mNE->deleteT(t, p)) return true;
	if(mSW->deleteT(t, p)) return true;
	if(mSE->deleteT(t, p)) return true;

	return false;
}

template<class T>
bool LineQuadTree<T>::update(T& t, const AABB& oldpos, const AABB& newpos)
{
	bool success = updateClean(t, oldpos, newpos);
	assert(success);
	return success;
}

template<class T>
void LineQuadTree<T>::clear()
{
	mLines.clear();
	if(mNW) {
		mNW->clear();
		mNE->clear();
		mSW->clear();
		mSE->clear();
	}
}

template<class T>
std::vector<T> LineQuadTree<T>::query(const AABB& area) const
{
	std::vector<T> retvalues;

	if(!mBoundary.intersects(area))
		return retvalues;

	for(auto& p : mLines) {
		if(area.intersects(p.second)) {
			retvalues.push_back(p.first);
		}
	}

	if(mNW == nullptr) {
		return retvalues;
	}

	{
		std::vector<T> moreretvalues = mNW->query(area);
		retvalues.insert(retvalues.end(), moreretvalues.begin(), moreretvalues.end());
	}

	{
		std::vector<T> moreretvalues = mNE->query(area);
		retvalues.insert(retvalues.end(), moreretvalues.begin(), moreretvalues.end());
	}

	{
		std::vector<T> moreretvalues = mSW->query(area);
		retvalues.insert(retvalues.end(), moreretvalues.begin(), moreretvalues.end());
	}

	{
		std::vector<T> moreretvalues = mSE->query(area);
		retvalues.insert(retvalues.end(), moreretvalues.begin(), moreretvalues.end());
	}

	return retvalues;
}

template<class T>
unsigned int LineQuadTree<T>::size() const
{
	unsigned int v = mLines.size();

	if(mNW) {
		v += mNW->size();
		v += mNE->size();
		v += mSW->size();
		v += mSE->size();
	}

	return v;
}

template<class T>
void LineQuadTree<T>::subdivide()
{
	assert(!mNW);
	assert(!mNE);
	assert(!mSW);
	assert(!mSE);
	float mx = mBoundary.halfDimension.x * 0.5f;
	float my = mBoundary.halfDimension.y * 0.5f;
	mNW = new LineQuadTree(AABB(Vector2(mBoundary.center.x - mx,
					mBoundary.center.y - my),
				Vector2(mx, my)));
	mNE = new LineQuadTree(AABB(Vector2(mBoundary.center.x + mx,
					mBoundary.center.y - my),
				Vector2(mx, my)));
	mSW = new LineQuadTree(AABB(Vector2(mBoundary.center.x - mx,
					mBoundary.center.y + my),
				Vector2(mx, my)));
	mSE = new LineQuadTree(AABB(Vector2(mBoundary.center.x + mx,
					mBoundary.center.y + my),
				Vector2(mx, my)));
}

template<class T>
bool LineQuadTree<T>::updateClean(T& t, const AABB& oldpos, const AABB& newpos)
{
	if(!deleteT(t, oldpos)) {
		std::cout << "LineQuadTree: failed to delete from position " << oldpos << "\n";
		assert(0);
		return false;
	}

	if(!insert(t, newpos)) {
		std::cout << "LineQuadTree: failed to insert to position " << newpos << "\n";
		assert(0);
		return false;
	}

	return true;
}

template<class T>
LineQuadTree<T>* LineQuadTree<T>::find(T& t, const AABB& pos)
{
	if(!mBoundary.contains(pos)) {
		return nullptr;
	}

	if(mLines.find(t) != mLines.end()) {
		return this;
	}

	if(!mNW) {
		return nullptr;
	}

	LineQuadTree* ret;
	ret = mNW->find(t, pos); if(ret) return ret;
	ret = mNE->find(t, pos); if(ret) return ret;
	ret = mSW->find(t, pos); if(ret) return ret;
	ret = mSE->find(t, pos); if(ret) return ret;

	return nullptr;
}

template<class T>
LQTIterator<T>::LQTIterator(LineQuadTree<T>& qt, bool atend)
{
	mQTs.push(&qt);
	if(atend) {
		mEnd = true;
	} else {
		mIt = qt.mLines.begin();
		// ensure valid iterator
		next();
	}
}

template<class T>
bool LQTIterator<T>::operator!=(const LQTIterator<T>& other) const
{
	return mEnd != other.mEnd;
}

template<class T>
LQTIterator<T>& LQTIterator<T>::operator++()
{
	assert(mIt != mQTs.front()->mLines.end());
	assert(!mEnd);
	++mIt;

	return next();
}

template<class T>
inline LQTIterator<T>& LQTIterator<T>::next()
{
	assert(!mEnd);

	while(mIt == mQTs.front()->mLines.end()) {
		auto oldtop = mQTs.front();
		if(oldtop->mNW) {
			mQTs.push(oldtop->mNW);
			mQTs.push(oldtop->mNE);
			mQTs.push(oldtop->mSW);
			mQTs.push(oldtop->mSE);
		}
		mQTs.pop();
		if(mQTs.empty()) {
			// end
			mEnd = true;
			return *this;
		}
		mIt = mQTs.front()->mLines.begin();
	}

	if(mIt == mQTs.front()->mLines.end()) {
		mEnd = true;
	}

	return *this;
}

template<class T>
LQTIterator<T> LineQuadTree<T>::begin()
{
	return LQTIterator<T>(*this, false);
}

template<class T>
LQTIterator<T> LineQuadTree<T>::end()
{
	return LQTIterator<T>(*this, true);
}

template<class T>
T LQTIterator<T>::operator*()
{
	return mIt->first;
}

}

#endif

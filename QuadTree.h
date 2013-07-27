#ifndef COMMON_QUADTREE_H
#define COMMON_QUADTREE_H

#include <cassert>
#include <cmath>

#include <iostream>
#include <map>
#include <queue>
#include <vector>

#include "Partition.h"

namespace Common {

template<class T>
class QuadTree;

template<class T>
class QTIterator {
	public:
		inline QTIterator(QuadTree<T>& qt, bool atend);
		inline bool operator!=(const QTIterator& other) const;
		inline T operator*();
		inline QTIterator& operator++();

	private:
		inline QTIterator& next();

		std::queue<QuadTree<T>*> mQTs;
		typename std::map<T, Vector2>::iterator mIt;
		bool mEnd = false;
};

template<class T>
class QuadTree {
	public:
		inline QuadTree(const AABB& boundary);
		inline ~QuadTree();
		inline bool insert(T& t, const Vector2& p); // invalidates all iterators
		inline bool deleteT(T& t, const Vector2& p); // invalidates all iterators
		inline bool update(T& t, const Vector2& oldpos, const Vector2& newpos); // invalidates all iterators - slow
		inline void clear();
		inline std::vector<T> query(const AABB& area) const;
		inline unsigned int size() const;
		inline QTIterator<T> begin();
		inline QTIterator<T> end();

	private:
		inline void subdivide();
		inline bool updateClean(T& t, const Vector2& oldpos, const Vector2& newpos);
		inline QuadTree<T>* find(T& t, const Vector2& pos);
		static const unsigned int NODE_CAPACITY = 4;
		constexpr static const float MIN_DIMENSION = 8.0f;
		AABB mBoundary;
		std::map<T, Vector2> mPoints;
		QuadTree* mNW;
		QuadTree* mNE;
		QuadTree* mSW;
		QuadTree* mSE;

		friend class QTIterator<T>;
};

template<class T>
QuadTree<T>::QuadTree(const AABB& boundary)
	: mBoundary(boundary),
	mNW(nullptr),
	mNE(nullptr),
	mSW(nullptr),
	mSE(nullptr)
{
}

template<class T>
QuadTree<T>::~QuadTree()
{
	if(mNW) {
		delete mNW;
		delete mNE;
		delete mSW;
		delete mSE;
	}
}

template<class T>
bool QuadTree<T>::insert(T& t, const Vector2& p)
{
	if(!mBoundary.contains(p)) {
		return false;
	}

	if(mPoints.size() < NODE_CAPACITY || mBoundary.halfDimension.x * 2.0f <= MIN_DIMENSION ||
			mBoundary.halfDimension.y * 2.0f <= MIN_DIMENSION) {
		mPoints[t] = p;
		return true;
	}

	if(mNW == nullptr) {
		subdivide();
	}

	if(mNW->insert(t, p)) return true;
	if(mNE->insert(t, p)) return true;
	if(mSW->insert(t, p)) return true;
	if(mSE->insert(t, p)) return true;

	return false;
}

template<class T>
bool QuadTree<T>::deleteT(T& t, const Vector2& p)
{
	if(!mBoundary.contains(p)) {
		return false;
	}

	auto it = mPoints.find(t);
	if(it != mPoints.end()) {
		mPoints.erase(it);
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
bool QuadTree<T>::update(T& t, const Vector2& oldpos, const Vector2& newpos)
{
	bool success = updateClean(t, oldpos, newpos);
	assert(success);
	return success;
}

template<class T>
void QuadTree<T>::clear()
{
	mPoints.clear();
	if(mNW) {
		mNW->clear();
		mNE->clear();
		mSW->clear();
		mSE->clear();
	}
}

template<class T>
std::vector<T> QuadTree<T>::query(const AABB& area) const
{
	std::vector<T> points;

	if(!mBoundary.intersects(area))
		return points;

	for(auto& p : mPoints) {
		if(area.contains(p.second)) {
			points.push_back(p.first);
		}
	}

	if(mNW == nullptr) {
		return points;
	}

	{
		std::vector<T> morepoints = mNW->query(area);
		points.insert(points.end(), morepoints.begin(), morepoints.end());
	}

	{
		std::vector<T> morepoints = mNE->query(area);
		points.insert(points.end(), morepoints.begin(), morepoints.end());
	}

	{
		std::vector<T> morepoints = mSW->query(area);
		points.insert(points.end(), morepoints.begin(), morepoints.end());
	}

	{
		std::vector<T> morepoints = mSE->query(area);
		points.insert(points.end(), morepoints.begin(), morepoints.end());
	}

	return points;
}

template<class T>
unsigned int QuadTree<T>::size() const
{
	unsigned int v = mPoints.size();

	if(mNW) {
		v += mNW->size();
		v += mNE->size();
		v += mSW->size();
		v += mSE->size();
	}

	return v;
}

template<class T>
void QuadTree<T>::subdivide()
{
	assert(!mNW);
	assert(!mNE);
	assert(!mSW);
	assert(!mSE);
	float mx = mBoundary.halfDimension.x * 0.5f;
	float my = mBoundary.halfDimension.y * 0.5f;
	mNW = new QuadTree(AABB(Vector2(mBoundary.center.x - mx,
					mBoundary.center.y - my),
				Vector2(mx, my)));
	mNE = new QuadTree(AABB(Vector2(mBoundary.center.x + mx,
					mBoundary.center.y - my),
				Vector2(mx, my)));
	mSW = new QuadTree(AABB(Vector2(mBoundary.center.x - mx,
					mBoundary.center.y + my),
				Vector2(mx, my)));
	mSE = new QuadTree(AABB(Vector2(mBoundary.center.x + mx,
					mBoundary.center.y + my),
				Vector2(mx, my)));
}

template<class T>
bool QuadTree<T>::updateClean(T& t, const Vector2& oldpos, const Vector2& newpos)
{
	if(!deleteT(t, oldpos)) {
		std::cout << "QuadTree: failed to delete from position " << oldpos << "\n";
		assert(0);
		return false;
	}

	if(!insert(t, newpos)) {
		std::cout << "QuadTree: failed to insert to position " << newpos << "\n";
		assert(0);
		return false;
	}

	return true;
}

template<class T>
QuadTree<T>* QuadTree<T>::find(T& t, const Vector2& pos)
{
	if(!mBoundary.contains(pos)) {
		return nullptr;
	}

	if(mPoints.find(t) != mPoints.end()) {
		return this;
	}

	if(!mNW) {
		return nullptr;
	}

	QuadTree* ret;
	ret = mNW->find(t, pos); if(ret) return ret;
	ret = mNE->find(t, pos); if(ret) return ret;
	ret = mSW->find(t, pos); if(ret) return ret;
	ret = mSE->find(t, pos); if(ret) return ret;

	return nullptr;
}


template<class T>
QTIterator<T>::QTIterator(QuadTree<T>& qt, bool atend)
{
	mQTs.push(&qt);
	if(atend) {
		mEnd = true;
	} else {
		mIt = qt.mPoints.begin();
		// ensure valid iterator
		next();
	}
}

template<class T>
bool QTIterator<T>::operator!=(const QTIterator<T>& other) const
{
	return mEnd != other.mEnd;
}

template<class T>
QTIterator<T>& QTIterator<T>::operator++()
{
	assert(mIt != mQTs.front()->mPoints.end());
	assert(!mEnd);
	++mIt;

	return next();

}

template<class T>
inline QTIterator<T>& QTIterator<T>::next()
{
	assert(!mEnd);

	while(mIt == mQTs.front()->mPoints.end()) {
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
		mIt = mQTs.front()->mPoints.begin();
	}

	if(mIt == mQTs.front()->mPoints.end()) {
		mEnd = true;
	}

	return *this;
}
	
template<class T>
QTIterator<T> QuadTree<T>::begin()
{
	return QTIterator<T>(*this, false);
}

template<class T>
QTIterator<T> QuadTree<T>::end()
{
	return QTIterator<T>(*this, true);
}

template<class T>
T QTIterator<T>::operator*()
{
	return mIt->first;
}

}

#endif

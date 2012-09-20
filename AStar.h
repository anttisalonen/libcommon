#ifndef COMMON_ASTAR_H
#define COMMON_ASTAR_H

#include <iostream>
#include <map>
#include <queue>
#include <list>
#include <set>
#include <functional>

namespace Common {

template<typename T>
class AStar {
	private:
		typedef std::function<std::set<T>(const T& a)> GraphFunc;
		typedef std::function<int(const T& a, const T& b)> CostFunc;
		typedef std::function<int(const T& a)> HeurFunc;
		typedef std::function<bool(const T& a)> GoalTestFunc;
		static inline void printPath(const std::list<T>& path);

	public:
		static inline std::list<T> solve(GraphFunc g, CostFunc c, HeurFunc h, 
			GoalTestFunc gtfunc, const T& start);
};

template<typename T>
class CompFunc {
	public:
		bool operator()(const std::pair<int, const T&>& lhs, const std::pair<int, const T&>& rhs)
		{
			return lhs.first > rhs.first;
		}
};

template<typename T>
std::list<T> AStar<T>::solve(GraphFunc g, CostFunc c, HeurFunc h, 
		GoalTestFunc gtfunc, const T& start)
{
	using namespace std;

	set<T> visited;
	std::map<T, int> cost_here; // real (g) cost
	list<T> path;
	std::map<T, T> parents;
	priority_queue<pair<int, T>, vector<pair<int, T> >, CompFunc<T>> open_nodes; // key is the total (f) cost

	open_nodes.push(make_pair(0, start));
	do {
		// current node is the parent
		T current(open_nodes.top().second);
		open_nodes.pop();

		// when relaxing the edges, previous edges are left in the queue
		// so check if already visited
		if(visited.find(current) != visited.end())
			continue;

		visited.insert(current);
		set<T> children = g(current);

		// check for goal
		if(gtfunc(current)) {
			path.push_front(current);
			break;
		}
		for(typename set<T>::const_iterator children_it = children.begin();
				children_it != children.end();
				++children_it) {
			// check if already visited
			if(visited.find(*children_it) != visited.end())
				continue;

			int edge_cost = c(current, *children_it);
			if(edge_cost < 0) {
				fprintf(stderr, "A* error: negative edge cost\n");
				continue;
			}
			int this_g_cost = cost_here[current] + edge_cost;

			// check if already in open list
			bool add_this_as_parent = true;
			typename std::map<T, int>::const_iterator already_open_child = cost_here.find(*children_it);
			if(already_open_child != cost_here.end()) {
				// already in open list => check if the cost is less than previous
				int cost_prev = already_open_child->second;
				if(cost_prev <= this_g_cost)
					add_this_as_parent = false;
			}
			if(add_this_as_parent) {
				int this_f_cost = this_g_cost + h(*children_it);
				parents[*children_it] = current;
				open_nodes.push(make_pair(this_f_cost, *children_it));
				cost_here[*children_it] = this_g_cost;
			}
		}
	} while(!open_nodes.empty());
	if(path.empty())
		return path;
	T curr_node = path.front();
	while(1) {
		typename std::map<T, T>::const_iterator par_it = parents.find(curr_node);
		if(par_it == parents.end())
			break;
		curr_node = par_it->second;
		path.push_back(curr_node);
	}
	path.reverse();
#ifdef DEBUG_ASTAR
	printPath(path);
#endif
	return path;
}

template<typename T>
void AStar<T>::printPath(const std::list<T>& path)
{
	std::cerr << "Found path: ";
	for(typename std::list<T>::const_iterator it = path.begin();
			it != path.end();
			++it) {
		std::cerr << *it << " ";
	}
	std::cerr << "\n";
}

}

#endif


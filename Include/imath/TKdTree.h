#pragma once

#include <vector>
#include <array>
#include <algorithm>
#include <functional>

#undef max

namespace imath
{

template<typename TPoint, uint8_t Dimensions>
class TKdTree
{
public:
	typedef std::array<double, Dimensions> Coordinate;
	typedef std::function<double(const TPoint& p, uint8_t index)> GetComponentFunc;
	typedef std::function<double(const Coordinate& x, const TPoint& y)> GetDistanceFunc;

private:
	GetComponentFunc m_getComponentFunc;
	GetDistanceFunc m_getDistanceFunc;

	struct Node
	{
		Node(const TPoint& pt) : m_point(pt), m_left(nullptr), m_right(nullptr)
		{
		}

		TPoint m_point;
		Node* m_left;
		Node* m_right;
	};

	class BoundedPriorityQueue
	{
	public:

		BoundedPriorityQueue() = delete;
		BoundedPriorityQueue(size_t bound) : bound(bound) { elements.reserve(bound + 1); };

		void push(const std::pair<double, const Node*>& val)
		{
			auto it = std::find_if(std::begin(elements), std::end(elements),
				[&val](std::pair<double, const Node*> element) { return val.first < element.first; });
			elements.insert(it, val);

			if (elements.size() > bound)
				elements.pop_back();
		}

		const std::pair<double, const Node*>& back() const { return elements.back(); };
		const std::pair<double, const Node*>& operator[](size_t index) const { return elements[index]; }
		size_t size() const { return elements.size(); }

	private:
		size_t bound;
		std::vector<std::pair<double, const Node*>> elements;
	};

	Node* m_root;
	std::vector<Node> m_nodes;

	struct NodeCmp
	{
		NodeCmp(uint8_t index, const GetComponentFunc& getComponentFunc) : m_index(index), m_getComponentFunc(getComponentFunc)
		{
		}
		bool operator()(const Node& a, const Node& b) const
		{
			return m_getComponentFunc(a.m_point, m_index) < m_getComponentFunc(b.m_point, m_index);
		}
		uint8_t m_index;
		const GetComponentFunc& m_getComponentFunc;
	};

	Node* MakeTree(size_t begin, size_t end, uint8_t index)
	{
		if (end <= begin)
			return nullptr;

		size_t n = begin + (end - begin) / 2;
		
		auto i = m_nodes.begin();
		std::nth_element(i + begin, i + n, i + end, NodeCmp(index, m_getComponentFunc));

		index = (index + 1) % Dimensions;

		m_nodes[n].m_left = MakeTree(begin, n, index);
		m_nodes[n].m_right = MakeTree(n + 1, end, index);
		return &m_nodes[n];
	}

	void Nearest(const Node* root, const Coordinate& point, uint8_t index, double& bestDistance, const Node*& best, double maxDistance) const
	{
		if (root == nullptr) {
			return;
		}

		const double d = m_getDistanceFunc(point, root->m_point);

		if (d < bestDistance) {
			bestDistance = d;

			if (bestDistance <= maxDistance) {
				best = root;
			}
		}

		if (bestDistance == 0) {
			return;
		}

		const double dx = m_getComponentFunc(root->m_point, index) - point[index];
		index = (index + 1) % Dimensions;

		Nearest(dx > 0 ? root->m_left : root->m_right, point, index, bestDistance, best, maxDistance);

		if (abs(dx) >= bestDistance || abs(dx) >= maxDistance) {
			return;
		}

		Nearest(dx > 0 ? root->m_right : root->m_left, point, index, bestDistance, best, maxDistance);
	}

	void KNearest(const Node* root, const Coordinate& point, uint8_t index, BoundedPriorityQueue& queue, size_t k) const
	{
		if (root == nullptr) {
			return;
		}

		const double d = m_getDistanceFunc(point, root->m_point);
		queue.push(std::make_pair(d, root));

		const double dx = m_getComponentFunc(root->m_point, index) - point[index];
		index = (index + 1) % Dimensions;

		KNearest(dx > 0 ? root->m_left : root->m_right, point, index, queue, k);

		if(queue.size() < k || abs(dx) < queue.back().first)
			KNearest(dx > 0 ? root->m_right : root->m_left, point, index, queue, k);
	}

	void InRadius(const Node* root, const Coordinate& point, const double radius, uint8_t index, std::vector<std::pair<const Node*, double>>& inRadius) const
	{
		if (root == nullptr) {
			return;
		}

		const double d = m_getDistanceFunc(point, root->m_point);

		if (d < radius) {
			inRadius.push_back(std::make_pair(root, d));
		}

		const double dx = m_getComponentFunc(root->m_point, index) - point[index];
		index = (index + 1) % Dimensions;

		InRadius(dx > 0 ? root->m_left : root->m_right, point, radius, index, inRadius);

		if (abs(dx) > radius) {
			return;
		}

		InRadius(dx > 0 ? root->m_right : root->m_left, point, radius, index, inRadius);
	}

public:
	TKdTree(const TKdTree&) = delete;
	TKdTree& operator=(const TKdTree&) = delete;

	TKdTree() : m_root(nullptr) 
	{
	}

	template<typename iterator>
	void MakeTree(iterator begin, iterator end, const GetComponentFunc& getComponentFunc, const GetDistanceFunc& getDistanceFunc)
	{
		m_getComponentFunc = getComponentFunc;
		m_getDistanceFunc = getDistanceFunc;
		m_nodes.clear();
		m_nodes.reserve(std::distance(begin, end));

		for (auto i = begin; i != end; ++i) {
			m_nodes.emplace_back(*i);
		}
		
		if (m_nodes.size() > 0) {
			m_root = MakeTree(0, m_nodes.size() - 1, 0);
		}
	}

	void MakeTree(const std::function<TPoint(size_t)>& construct, size_t n, const GetComponentFunc& getComponentFunc, const GetDistanceFunc& getDistanceFunc)
	{
		m_getComponentFunc = getComponentFunc;
		m_getDistanceFunc = getDistanceFunc;
		m_nodes.clear();
		m_nodes.reserve(n);

		for (size_t i = 0; i < n; ++i) {
			m_nodes.emplace_back(construct(i));
		}

		if (m_nodes.size() > 0) {
			m_root = MakeTree(0, m_nodes.size(), 0);
		}
	}

	bool Empty() const
	{
		return m_nodes.empty();
	}

	bool Nearest(const Coordinate& pt, TPoint& p, double& resultDistance, double maxDistance = std::numeric_limits<double>::max()) const
	{
		if (m_root == nullptr) {
			return false;
		}

		const Node* best = nullptr;
		double dist = std::numeric_limits<double>::max();
		Nearest(m_root, pt, 0, dist, best, maxDistance);

		if (best != nullptr) {
			p = best->m_point;
			resultDistance = dist;
			return true;
		}

		return false;
	}

	bool KNearest(const Coordinate& pt, std::vector<std::pair<TPoint, double>>& neighborsWithDistance, size_t k) const
	{
		if (m_root == nullptr) {
			return false;
		}

		if (k == 0) {
			return true;
		}

		BoundedPriorityQueue queue(k);
		KNearest(m_root, pt, 0, queue, k);
		neighborsWithDistance.resize(queue.size());

		for (size_t i = 0; i < queue.size(); ++i) {
			neighborsWithDistance[i] = std::make_pair(queue[i].second->m_point, queue[i].first);
		}

		return true;
	}

	bool InRadius(const Coordinate& pt, double radius, std::vector<std::pair<TPoint, double>>& pointsWithDistance) const
	{
		if (m_root == nullptr) {
			return false;
		}

		std::vector<std::pair<const Node*, double>> inRadiusNode;
		InRadius(m_root, pt, radius, 0, inRadiusNode);
		pointsWithDistance.resize(inRadiusNode.size());

		for (size_t i = 0; i < inRadiusNode.size(); ++i) {
			pointsWithDistance[i] = std::make_pair(inRadiusNode[i].first->m_point, inRadiusNode[i].second);
		}

		return true;
	}
};

} // namespace imath



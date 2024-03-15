#pragma once
#include <stdexcept>
#include <initializer_list>

namespace Utils {
	// If there were duplicate keys, then the first one that occurs from head to tail will be returned
	template<typename A, typename B, size_t N>
	class Map {
		static_assert(!std::is_same<A, B>::value, "A and B must be different types.");
	public:
		std::pair<A, B> m_map[N];
	public:
		Map(std::initializer_list<std::pair<A, B> > list) {
			if (list.size() != N) {
				throw std::invalid_argument("Size of the initializer list doesn't match N!");
			}
			size_t i = 0;
			for (const auto& e : list) {
				m_map[i].first = e.first;
				m_map[i].second = e.second;
				++i;
			}
		}

		const A& operator[](const B& b) const {
			for (size_t i = 0; i < N; ++i) {
				if (m_map[i].second == b) {
					return m_map[i].first;
				}
			}
		}
		const B& operator[](const A& a) const {
			for (size_t i = 0; i < N; ++i) {
				if (m_map[i].first == a) {
					return m_map[i].second;
				}
			}
		}

		const A& get(const B& b, const A& defaultRet) const {
			for (size_t i = 0; i < N; ++i) {
				if (m_map[i].second == b) {
					return m_map[i].first;
				}
			}
			return defaultRet;
		}
		const B& get(const A& a, const B& defaultRet) const {
			for (size_t i = 0; i < N; ++i) {
				if (m_map[i].first == a) {
					return m_map[i].second;
				}
			}
			return defaultRet;
		}

		const size_t count(const A& a) const {
			size_t n_count = 0;
			for (size_t i = 0; i < N; ++i) {
				if (m_map[i].first == a) {
					++n_count;
				}
			}
			return n_count;
		}
		const size_t count(const B& b) const {
			size_t n_count = 0;
			for (size_t i = 0; i < N; ++i) {
				if (m_map[i].second == b) {
					++n_count;
				}
			}
			return n_count;
		}
	};
}
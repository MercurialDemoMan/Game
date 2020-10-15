#pragma once

#include <functional>
#include <optional>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "Types.hpp"


namespace Engine3D
{
	/**
	 * bounding box
	 */
	struct BoundingBox
	{
		BoundingBox() {}
		BoundingBox(const glm::vec3& min_corner, const glm::vec3& max_corner) : min(min_corner), max(max_corner) {}
		BoundingBox(const glm::vec3&& min_corner, const glm::vec3&& max_corner) : min(min_corner), max(max_corner) {}

		glm::vec3 max;
		glm::vec3 min;
	};

	/**
	 * structure that divides space into blocks and manages big number of objects in them
	 */
	template <typename T>
	class SpatialPartition
	{
	public:

		static constexpr u32 DefaultCellSize = 16;

		/**
		 * constructors
		 */
		SpatialPartition(u32 cell_size = DefaultCellSize) : m_cell_size(cell_size) {}
		SpatialPartition(u32 cell_size, std::function<BoundingBox(const T&)> get_bounding_box) : m_get_bounding_box(std::move(get_bounding_box)) { }

		/**
		 * explicit constructor
		 */
		void init(u32 cell_size = DefaultCellSize)
		{
			m_cells.clear();
			m_cell_size = cell_size;
		}
		void init(u32 cell_size, std::function<BoundingBox(const T&)> get_bounding_box)
		{
			m_cells.clear();
			m_cell_size = cell_size;
			m_get_bounding_box = get_bounding_box;
		}

		/**
		 * delete cell data
		 */
		void clear()
		{
			m_cells.clear();
		}

		/**
		 * add object into the space
		 */
		void add(const T* object)
		{
			if (m_get_bounding_box)
			{
				BoundingBox object_rect = m_get_bounding_box(*object);
				
				glm::vec3 cell_max = m_world_to_cell(object_rect.max);
				glm::vec3 cell_min = m_world_to_cell(object_rect.min);

				for (s64 x = static_cast<s64>(cell_min.x); x < static_cast<s64>(cell_max.x) + 1; x++)
				{
					for (s64 y = static_cast<s64>(cell_min.y); y < static_cast<s64>(cell_max.y) + 1; y++)
					{
						for (s64 z = static_cast<s64>(cell_min.z); z < static_cast<s64>(cell_max.z) + 1; z++)
						{
							u64 hash = m_hash_func(glm::vec3(x, y, z));

							auto it = m_cells.find(hash);

							if (it == m_cells.end())
							{
								m_cells.emplace(hash, std::vector<T*>());
							}

							m_cells[hash].push_back(const_cast<T*>(object));
						}
					}
				}
			}
			else
			{
				throw std::runtime_error("SpatialPartition::add() error: bounding box function not specified");
			}
		}

		/**
		 * delete object from space
		 */
		void del(const T* object)
		{
			if (m_get_bounding_box)
			{
				BoundingBox object_rect = m_get_bounding_box(*object);

				glm::vec3 cell_max = m_world_to_cell(object_rect.max);
				glm::vec3 cell_min = m_world_to_cell(object_rect.min);

				for (s32 x = cell_min.x; x < cell_max.x + 1; x++)
				{
					for (s32 y = cell_min.y; y < cell_max.y + 1; y++)
					{
						for (s32 z = cell_min.z; z < cell_max.z + 1; z++)
						{
							u64 hash = m_hash_func(glm::vec3(x, y, z));

							auto cell = m_cells.find(hash);

							if (cell != m_cells.end())
							{
								for (auto it = cell->second.begin(); it != cell->second.end(); it++)
								{
									if (*it == object)
									{
										cell->erase(it);
										return;
									}
								}
							}
						}
					}
				}
			}
			else
			{
				throw std::runtime_error("SpatialPartition::del() error: bounding box function not specified");
			}
		}

		/**
		 * update cells
		 *
		 * \note - slow
		 */
		void update(const std::vector<T>& objects)
		{
			if (m_get_bounding_box)
			{
				clear();

				for (auto& object : objects)
				{
					add(&object);
				}
			}
			else
			{
				throw std::runtime_error("SpatialPartition::update() error: bounding box function not specified");
			}
		}
		void update(const std::vector<T*>& objects)
		{
			if (m_get_bounding_box)
			{
				clear();

				for (auto& object : objects)
				{
					add(object);
				}
			}
			else
			{
				throw std::runtime_error("SpatialPartition::update() error: bounding box function not specified");
			}
		}

		/**
		 * get cell rectangle
		 */
		BoundingBox getCell(const glm::vec3& pos)
		{
			glm::vec3 cell_pos = m_world_to_cell(pos);

			return { cell_pos * static_cast<float>(m_cell_size), (cell_pos + 1.0f) * static_cast<float>(m_cell_size) };
		}

		/**
		 * get cell size
		 */
		u32 getCellSize()
		{
			return m_cell_size;
		}

		/**
		 * get the objects within a certain cell
		 */
		const std::vector<T*>* get(const glm::vec3& pos)
		{
			glm::vec3 cell_pos = m_world_to_cell(pos);

			auto cell = m_cells.find(m_hash_func(cell_pos));

			if (cell == m_cells.end())
			{
				return nullptr;
			}
			else
			{
				return &cell->second;
			}
		}

	private:

		std::hash<glm::vec3> m_hash_func {};

		std::function<glm::vec3(const glm::vec3&)> m_world_to_cell = [this](const glm::vec3& pos) -> glm::vec3
		{
			return glm::vec3(floor(pos.x / m_cell_size), floor(pos.y / m_cell_size), floor(pos.z / m_cell_size));
		};

		std::unordered_map<u64, std::vector<T*>> m_cells;
		u32                           m_cell_size;
		std::function<BoundingBox(const T&)> m_get_bounding_box;
	};
}
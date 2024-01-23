// AUTOGENERATED COPYRIGHT HEADER START
// Copyright (C) 2017-2023 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
// AUTOGENERATED COPYRIGHT HEADER END

#pragma once
#include "common.hpp"
#include "util/util-pool.hpp"
#include "util/util-singleton.hpp"

#include "warning-disable.hpp"
#include <chrono>
#include <cinttypes>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>

#include <graphics/graphics.h>
#include "warning-enable.hpp"

static bool operator<(gs_sampler_info const& lhs, gs_sampler_info const& rhs) noexcept
{
	return (lhs.address_u < rhs.address_u) || (lhs.address_v < rhs.address_v) || (lhs.address_w < rhs.address_w) || (lhs.filter < rhs.filter) || (lhs.max_anisotropy < rhs.max_anisotropy);
}

static bool operator==(gs_sampler_info const& lhs, gs_sampler_info const& rhs) noexcept
{
	return (lhs.address_u == rhs.address_u) && (lhs.address_v == rhs.address_v) && (lhs.address_w == rhs.address_w) && (lhs.border_color == rhs.border_color) && (lhs.filter == rhs.filter) && (lhs.max_anisotropy == rhs.max_anisotropy);
}

template<>
struct std::hash<gs_sampler_info> {
	std::size_t operator()(gs_sampler_info si) const noexcept
	{
		auto hash = std::hash<uint8_t>{}(si.address_u);
		hash ^= std::hash<uint8_t>{}(si.address_v);
		hash ^= std::hash<uint8_t>{}(si.address_w);
		hash ^= std::hash<uint32_t>{}(si.border_color);
		hash ^= std::hash<uint8_t>{}(si.filter);
		hash ^= std::hash<uint8_t>{}(si.max_anisotropy);
		return hash;
	}
};

namespace streamfx::obs::gs {
	class sampler {
		gs_sampler_info   _info;
		gs_sampler_state* _state;

		public:
		~sampler();

		sampler();

		sampler(gs_sampler_info si);

		FORCE_INLINE gs_sample_filter get_filter() const
		{
			return _info.filter;
		}

		FORCE_INLINE gs_address_mode get_address_mode_u() const
		{
			return _info.address_u;
		}

		FORCE_INLINE gs_address_mode get_address_mode_v() const
		{
			return _info.address_v;
		}

		FORCE_INLINE gs_address_mode get_address_mode_w() const
		{
			return _info.address_w;
		}

		FORCE_INLINE int get_max_anisotropy() const
		{
			return _info.max_anisotropy;
		}

		FORCE_INLINE uint32_t get_border_color() const
		{
			return _info.border_color;
		}
		FORCE_INLINE uint8_t get_border_color(bool r, bool g, bool b, bool a) const
		{
			if (a)
				return (_info.border_color >> 24) & 0xFF;
			if (r)
				return (_info.border_color >> 16) & 0xFF;
			if (g)
				return (_info.border_color >> 8) & 0xFF;
			if (b)
				return _info.border_color & 0xFF;
			return 0;
		}

		FORCE_INLINE gs_sampler_info info() const
		{
			return _info;
		}

		FORCE_INLINE gs_sampler_state* update() const
		{
			return _state;
		}

		public:
		class pool;
		typedef gs_sampler_info                                                                                            _pool_key_t;
		typedef streamfx::util::multipool<streamfx::obs::gs::sampler::pool, streamfx::obs::gs::sampler, 1000, _pool_key_t> _pool_t;

		class pool : public _pool_t {
			friend streamfx::util::singleton<streamfx::obs::gs::sampler::pool>;
			friend _pool_t;

			public:
			~pool() {}

			protected:
			pool() : _pool_t() {}

			static _pool_key_t as_key(streamfx::obs::gs::sampler* to_hash);

			static _pool_key_t as_key(gs_sampler_info to_hash);
		};
	};
} // namespace streamfx::obs::gs

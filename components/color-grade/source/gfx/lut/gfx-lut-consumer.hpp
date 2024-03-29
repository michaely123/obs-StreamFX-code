// AUTOGENERATED COPYRIGHT HEADER START
// Copyright (C) 2020-2023 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
// AUTOGENERATED COPYRIGHT HEADER END

#pragma once
#include "gfx-lut.hpp"
#include "obs/gs/gs-effect.hpp"
#include "obs/gs/gs-texture.hpp"

#include "warning-disable.hpp"
#include <memory>
#include "warning-enable.hpp"

namespace streamfx::gfx::lut {
	class consumer {
		std::shared_ptr<streamfx::gfx::lut::data> _data;

		public:
		consumer();
		~consumer();

		std::shared_ptr<streamfx::obs::gs::effect> prepare(streamfx::gfx::lut::color_depth depth, std::shared_ptr<streamfx::obs::gs::texture> lut);

		void consume(streamfx::gfx::lut::color_depth depth, std::shared_ptr<streamfx::obs::gs::texture> lut, std::shared_ptr<streamfx::obs::gs::texture> texture);
	};
} // namespace streamfx::gfx::lut

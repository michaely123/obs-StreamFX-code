// AUTOGENERATED COPYRIGHT HEADER START
// Copyright (C) 2020-2023 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
// AUTOGENERATED COPYRIGHT HEADER END

#pragma once
#include "obs/gs/gs-effect.hpp"
#include "obs/gs/gs-vertexbuffer.hpp"

#include "warning-disable.hpp"
#include <memory>
#include "warning-enable.hpp"

namespace streamfx::gfx {
	class util {
		std::shared_ptr<::streamfx::obs::gs::effect>        _effect;
		std::shared_ptr<::streamfx::obs::gs::vertexbuffer> _point_vb;
		std::shared_ptr<::streamfx::obs::gs::vertexbuffer> _line_vb;
		std::shared_ptr<::streamfx::obs::gs::vertexbuffer> _arrow_vb;
		std::shared_ptr<::streamfx::obs::gs::vertexbuffer> _quad_vb;
		std::shared_ptr<::streamfx::obs::gs::vertexbuffer> _fstri_vb;

		public /* Singleton */:
		static std::shared_ptr<streamfx::gfx::util> get();

		private:
		util();

		public:
		~util();

		void draw_point(float x, float y, uint32_t color = 0xFFFFFFFF);

		void draw_line(float x, float y, float x2, float y2, uint32_t color = 0xFFFFFFFF);

		void draw_arrow(float x, float y, float x2, float y2, float w = 0., uint32_t color = 0xFFFFFFFF);

		void draw_rectangle(float x, float y, float w, float h, bool frame, uint32_t color = 0xFFFFFFFF);

		void draw_fullscreen_triangle();
	};
} // namespace streamfx::gfx

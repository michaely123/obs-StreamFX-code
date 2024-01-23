// AUTOGENERATED COPYRIGHT HEADER START
// Copyright (C) 2020-2023 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
// AUTOGENERATED COPYRIGHT HEADER END

#include "warning-disable.hpp"
#include <memory>
#include "warning-enable.hpp"

namespace streamfx::gfx {
	class opengl {
		public /* Singleton */:
		static std::shared_ptr<streamfx::gfx::opengl> get();

		private:
		opengl();

		public:
		~opengl();
	};
} // namespace streamfx::gfx

// AUTOGENERATED COPYRIGHT HEADER START
// Copyright (C) 2017-2023 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
// AUTOGENERATED COPYRIGHT HEADER END

#include "gs-texture.hpp"

static uint32_t decode_flags(streamfx::obs::gs::texture_flags texture_flags)
{
	uint32_t flags = 0;
	if (has(texture_flags, streamfx::obs::gs::texture_flags::Dynamic))
		flags |= GS_DYNAMIC;
	if (has(texture_flags, streamfx::obs::gs::texture_flags::BuildMipMaps))
		flags |= GS_BUILD_MIPMAPS;
	if (has(texture_flags, streamfx::obs::gs::texture_flags::Shared))
		flags |= GS_SHARED_TEX;
	if (has(texture_flags, streamfx::obs::gs::texture_flags::GlobalShared))
		flags |= GS_SHARED_KM_TEX;
	return flags;
}

streamfx::obs::gs::texture::texture(uint32_t width, uint32_t height, gs_color_format format, uint32_t mip_levels, const uint8_t** mip_data, streamfx::obs::gs::texture_flags flags)
{
	_is_owner = true;
	_flags    = flags;
	_file     = std::filesystem::path();

#ifdef _DEBUG
	if (width == 0)
		throw std::logic_error("width must be at least 1");
	if (height == 0)
		throw std::logic_error("height must be at least 1");
	if (mip_levels == 0)
		throw std::logic_error("mip_levels must be at least 1");

	if (mip_levels > 1 || ((flags & texture_flags::BuildMipMaps) == texture_flags::BuildMipMaps)) {
		bool isPOT = streamfx::util::math::is_power_of_two(width) && streamfx::util::math::is_power_of_two(height);
		if (!isPOT)
			throw std::logic_error("Texture dimensions must be a power of two.");
	}
#endif

	{
		auto gctx = streamfx::obs::gs::context();
		_texture  = gs_texture_create(width, height, format, mip_levels, mip_data, decode_flags(flags));
	}

	if (!_texture)
		throw std::runtime_error("Failed to create texture.");
}

streamfx::obs::gs::texture::texture(std::filesystem::path file)
{
	_is_owner = true;
	_file     = file;
	_flags    = texture_flags::None;
	_file     = std::filesystem::path();

	auto gctx = streamfx::obs::gs::context();
	_texture  = gs_texture_create_from_file(file.generic_string().c_str());

	if (!_texture)
		throw std::runtime_error("Failed to load texture.");
}

streamfx::obs::gs::texture::~texture()
{
	if (_is_owner && _texture) {
		auto gctx = streamfx::obs::gs::context();
		gs_texture_destroy(_texture);
	}
	_texture = nullptr;
}

static std::shared_ptr<streamfx::obs::gs::texture::pool> loader_instance;

static auto loader = streamfx::loader(
	[]() { // Initializer
		loader_instance = streamfx::obs::gs::texture::pool::instance();
	},
	[]() { // Finalizer
		loader_instance.reset();
	},
	streamfx::loader_priority::HIGHER);

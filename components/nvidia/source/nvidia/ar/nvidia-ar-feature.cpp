// AUTOGENERATED COPYRIGHT HEADER START
// Copyright (C) 2021-2023 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
// AUTOGENERATED COPYRIGHT HEADER END

#include "nvidia/ar/nvidia-ar-feature.hpp"
#include "obs/gs/gs-helper.hpp"
#include "util/util-logging.hpp"
#include "util/util-platform.hpp"

#ifdef _DEBUG
#define ST_PREFIX "<%s> "
#define D_LOG_ERROR(x, ...) P_LOG_ERROR(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#define D_LOG_WARNING(x, ...) P_LOG_WARN(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#define D_LOG_INFO(x, ...) P_LOG_INFO(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#define D_LOG_DEBUG(x, ...) P_LOG_DEBUG(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#else
#define ST_PREFIX "<nvidia::ar::feature> "
#define D_LOG_ERROR(...) P_LOG_ERROR(ST_PREFIX __VA_ARGS__)
#define D_LOG_WARNING(...) P_LOG_WARN(ST_PREFIX __VA_ARGS__)
#define D_LOG_INFO(...) P_LOG_INFO(ST_PREFIX __VA_ARGS__)
#define D_LOG_DEBUG(...) P_LOG_DEBUG(ST_PREFIX __VA_ARGS__)
#endif

streamfx::nvidia::ar::feature::~feature()
{
	D_LOG_DEBUG("Finalizing... (Addr: 0x%" PRIuPTR ")", this);
}

streamfx::nvidia::ar::feature::feature(feature_t feature) : _nvcuda(::streamfx::nvidia::cuda::obs::get()), _nvcv(::streamfx::nvidia::cv::cv::get()), _nvar(::streamfx::nvidia::ar::ar::get()), _fx()
{
	D_LOG_DEBUG("Initializating... (Addr: 0x%" PRIuPTR ")", this);
	auto gctx = ::streamfx::obs::gs::context();
	auto cctx = cuda::obs::get()->get_context()->enter();

	// Create the Effect/Feature.
	::streamfx::nvidia::ar::handle_t handle;
	if (cv::result res = _nvar->NvAR_Create(feature, &handle); res != cv::result::SUCCESS) {
		throw cv::exception("Failed to create feature.", res);
	}
	_fx = std::shared_ptr<void>(handle, [this](::streamfx::nvidia::ar::handle_t handle) { _nvar->NvAR_Destroy(handle); });

	// Set CUDA stream and model directory.
	set_cuda_stream(P_NVAR_CONFIG "CUDAStream", _nvcuda->get_stream());
	_model_path = _nvar->get_model_path().generic_u8string();
	set_string(P_NVAR_CONFIG "ModelDir", _model_path);
}

streamfx::nvidia::cv::result streamfx::nvidia::ar::feature::get(parameter_t param, std::string_view& value)
{
	const char* cvalue = nullptr;
	cv::result  res    = get_string(param, cvalue);
	if (res == cv::result::SUCCESS) {
		if (cvalue) {
			value = std::string_view(cvalue);
		} else {
			value = std::string_view();
		}
	}
	return res;
}

streamfx::nvidia::cv::result streamfx::nvidia::ar::feature::get(parameter_t param, std::string& value)
{
	const char* cvalue = nullptr;
	cv::result  res    = get_string(param, cvalue);
	if (res == cv::result::SUCCESS) {
		if (cvalue) {
			value = cvalue;
		} else {
			value.clear();
		}
	}
	return res;
}

streamfx::nvidia::cv::result streamfx::nvidia::ar::feature::get(parameter_t param, std::u8string& value)
{
	const char8_t* cvalue = nullptr;
	cv::result  res    = get_string(param, cvalue);
	if (res == cv::result::SUCCESS) {
		if (cvalue) {
			value = cvalue;
		} else {
			value.clear();
		}
	}
	return res;
}

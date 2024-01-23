#!/usr/bin/env bash
shopt -s nocasematch

# Variables
opt_generator="Ninja Multi-config"
opt_build_dir="${PWD}/build"
opt_install_dir="${PWD}/build/install"
opt_package_dir="${PWD}/build/package"
opt_package_name=""

# Figure out the platform we are on.
platform="unknown"
if [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "msys" ]]; then
	platform="windows"
elif [[ "$OSTYPE" == "freebsd"* ]] || [[ "$OSTYPE" == "linux"* ]]; then
	platform="linux"
else
	platform="mac"
fi

function group_start {
	if [[ "${GITHUB_ACTIONS}" ]]; then echo "##[group]$1"; fi
}
function group_end {
	if [[ "${GITHUB_ACTIONS}" ]]; then echo "##[endgroup]"; fi
}

function if_value {
	if [[ ${1} ]]; then echo ${2}; fi
}

function task_prerequisites {
	if [[ "$platform" == "mac" ]]; then
		brew install cmake
		if [[ $? != 0 ]]; then return $?; fi
	elif [[ "$platform" == "linux" ]]; then
        sudo apt-get -qq update
		if [[ $? != 0 ]]; then return $?; fi
		sudo apt-get -y install \
			build-essential \
			cmake \
			git \
			libavcodec-dev \
			libavdevice-dev \
			libavfilter-dev \
			libavformat-dev \
			libavutil-dev \
			libcurl4-openssl-dev \
			libqt6svg6-dev \
			libswresample-dev \
			libswscale-dev \
			ninja-build \
			pkg-config \
			qt6-base-dev \
			qt6-base-private-dev \
			qt6-image-formats-plugins \
			qt6-wayland
		if [[ $? != 0 ]]; then return $?; fi
	fi
}

function task_fetch {
	#git fetch --all --tags
	git submodule update --init --force --recursive
	return 0
}

function task_clean {
	git clean -xfd
	git submodule foreach --recursive git clean -xfd
	git reset --hard
	git submodule foreach --recursive git reset --hard
	return 0
}

function task_patch {
	function find_apply_patches {
		patch_dir=$1
		target_dir=$2

		if [[ ! -d "${patch_dir}" ]]; then
			return 1
		fi

		echo "Searching tree '${patch_dir}'..."
		for p in "${patch_dir}"/*; do
			pf=$(basename "$p")

			# Skip platform specific directories
			if [[ "${pf}" == "windows" ]] || [[ "${pf}" == "linux" ]] || [[ "${pf}" == "mac" ]] || [[ "${pf}" == "unknown" ]]; then
				continue
			fi

			if [ -f "${p}" ]; then
				# This is a file, so apply it.
				echo "Applying patch '${p}'..."
				$(cd "${target_dir}/" && git apply "${p}")
			elif [ -d "${p}" ]; then
				find_apply_patches "${patch_dir}/${pf}" "${target_dir}/${pf}"
			fi
		done

		return 0
	}

	find_apply_patches "${PWD}/patches" "${PWD}"
	find_apply_patches "${PWD}/patches/${platform}" "${PWD}"

	return 0
}

function task_libobs {
	if [[ "$platform" == "linux" ]]; then
		group_start "Install Pre-requisites"
		sudo apt-get -y install \
			build-essential \
			pkg-config \
			cmake \
			git \
			libavcodec-dev \
			libavdevice-dev \
			libavfilter-dev \
			libavformat-dev \
			libavutil-dev \
			libswresample-dev \
			libswscale-dev \
			libcurl4-openssl-dev \
			libmbedtls-dev \
			libjansson-dev \
			libgl1-mesa-dev \
			libx11-dev \
			libxcb-randr0-dev \
			libxcb-shm0-dev \
			libxcb-xinerama0-dev \
			libxcb-composite0-dev \
			libxcomposite-dev \
			libxinerama-dev \
			libxcb1-dev \
			libx11-xcb-dev \
			libxcb-xfixes0-dev \
			libxss-dev \
			libglvnd-dev \
			libgles2-mesa \
			libgles2-mesa-dev \
			libwayland-dev \
			libasound2-dev \
			libjack-jackd2-dev \
			libpulse-dev \
			libsndio-dev \
			libasio-dev \
			libasound2-dev \
			libfontconfig-dev \
			libjack-jackd2-dev \
			libpulse-dev \
			libsndio-dev \
			libudev-dev \
			libv4l-dev \
			libva-dev \
			libvpl-dev \
			libdrm-dev \
			libasio-dev
		if [[ $? != 0 ]]; then return $?; fi
		group_end
	elif [[ "$platform" == "mac" ]]; then
		group_start "Install Pre-requisites"
		brew install git
		if [[ $? != 0 ]]; then return $?; fi
		brew install cmake
		if [[ $? != 0 ]]; then return $?; fi
		group_end
	fi

	group_start "Configure"
	if [[ "$platform" == "linux" ]]; then
		cmake \
			-S "${PWD}/third-party/obs-studio/" \
			--preset linux-x86_64 \
			-G "Unix Makefiles" \
			-B "${PWD}/third-party/obs-studio/build" \
			-DCMAKE_BUILD_TYPE=RelWithDebInfo \
			-DENABLE_UI=OFF \
			-DENABLE_PLUGINS=OFF \
			-DENABLE_SCRIPTING=OFF \
			-DENABLE_BROWSER=ON \
			-DOBS_VERSION_OVERRIDE=$(cd "${PWD}/third-party/obs-studio/" && git describe --tags HEAD)
		if [[ $? != 0 ]]; then return $?; fi
	elif [[ "$platform" == "mac" ]]; then
		cmake \
			-S "${PWD}/third-party/obs-studio/" \
			--preset macos \
			-G "Xcode" \
			-B "${PWD}/third-party/obs-studio/build" \
			-DCMAKE_BUILD_TYPE=RelWithDebInfo \
			-DENABLE_UI=OFF \
			-DENABLE_PLUGINS=OFF \
			-DENABLE_SCRIPTING=OFF \
			-DENABLE_BROWSER=ON \
			-DOBS_VERSION_OVERRIDE=$(cd "${PWD}/third-party/obs-studio/" && git describe --tags HEAD) \
			-DCMAKE_OSX_DEPLOYMENT_TARGET=10.15 \
			-DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"
		if [[ $? != 0 ]]; then return $?; fi
	else
		cmake \
			-S "${PWD}/third-party/obs-studio/" \
			--preset windows-x64 \
			-B "${PWD}/third-party/obs-studio/build" \
			-DCMAKE_BUILD_TYPE=RelWithDebInfo \
			-DENABLE_UI=OFF \
			-DENABLE_PLUGINS=OFF \
			-DENABLE_SCRIPTING=OFF \
			-DENABLE_BROWSER=ON \
			-DOBS_VERSION_OVERRIDE=$(cd "${PWD}/third-party/obs-studio/" && git describe --tags HEAD)
		if [[ $? != 0 ]]; then return $?; fi
	fi
	if [[ $? != 0 ]]; then return $?; fi
	group_end

	group_start "Build"
	cmake \
		--build "${PWD}/third-party/obs-studio/build" \
		--parallel \
		--config RelWithDebInfo \
		--target obs-frontend-api
	if [[ $? != 0 ]]; then return $?; fi
	group_end

	group_start "Install"
	if [[ "$platform" == "linux" ]]; then
		cmake \
			--install "${PWD}/third-party/obs-studio/build" \
			--prefix "${PWD}/third-party/obs-studio/build/install" \
			--config RelWithDebInfo \
			--component obs_libraries
		if [[ $? != 0 ]]; then return $?; fi
	else
		cmake \
			--install "${PWD}/third-party/obs-studio/build" \
			--prefix "${PWD}/third-party/obs-studio/build/install" \
			--config RelWithDebInfo \
			--component Development
		if [[ $? != 0 ]]; then return $?; fi
	fi
	group_end
}

function task_build {
	if [[ "$platform" == "windows" ]]; then
		suffix=x64
	elif [[ "$platform" == "linux" ]]; then
		suffix=x86_64
	else
		suffix=universal
	fi

	libobs_dir=(${PWD}/third-party/obs-studio/build/install/)
	ffmpeg_dir=(${PWD}/third-party/obs-studio/.deps/obs-deps-*-${suffix}/)
	curl_dir=(${PWD}/third-party/obs-studio/.deps/obs-deps-*-${suffix}/)
	qt_dir=(${PWD}/third-party/obs-studio/.deps/obs-deps-qt6-*-${suffix}/)
	if [[ "$platform" == "mac" ]]; then
		# obs-deps/qt6's AutoMOC has an unpatched bug: https://bugreports.qt.io/browse/QTBUG-117765
		group_start "Fix unpatched Qt6.5.3 bug in upstream obs-deps"

		brew install qt@6
		if [[ $? != 0 ]]; then return $?; fi
		rm "${qt_dir}/libexec/moc"
		if [[ $? != 0 ]]; then return $?; fi
		ln -s "$(brew ls -q qt | grep libexec/moc)" "${qt_dir}/libexec/moc"
		if [[ $? != 0 ]]; then return $?; fi

		group_end
	fi

	group_start "Configure"
	if [[ "$platform" == "linux" ]]; then
		cmake \
			-G "Ninja Multi-Config" \
			-S ${PWD} \
			-B ${opt_build_dir} \
			-DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
			-DCMAKE_INSTALL_PREFIX="${opt_install_dir}" \
			-DPACKAGE_NAME="${opt_package_name}" \
			-DPACKAGE_PREFIX="${opt_package_dir}" \
			-Dlibobs_DIR="${PWD}/third-party/obs-studio/build/install" \
			-DFFmpeg_DIR="${ffmpeg_dir}" \
			-DCURL_DIR="${curl_dir}" \
			-DQt6_DIR="${qt_dir}"
		if [[ $? != 0 ]]; then return $?; fi
	elif [[ "$platform" == "mac" ]]; then
		cmake \
			-G "Xcode" \
			-S ${PWD} \
			-B ${opt_build_dir} \
			-DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
			-DCMAKE_INSTALL_PREFIX="${opt_install_dir}" \
			-DPACKAGE_NAME="${opt_package_name}" \
			-DPACKAGE_PREFIX="${opt_package_dir}" \
			-Dlibobs_DIR="${PWD}/third-party/obs-studio/build/install" \
			-DFFmpeg_DIR="${ffmpeg_dir}" \
			-DCURL_DIR="${curl_dir}" \
			-DQt6_DIR="${qt_dir}" \
			-DCMAKE_OSX_DEPLOYMENT_TARGET=10.15 \
			-DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"
		if [[ $? != 0 ]]; then return $?; fi
	else
		cmake \
			-S ${PWD} \
			-B ${opt_build_dir} \
			-DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
			-DCMAKE_INSTALL_PREFIX="${opt_install_dir}" \
			-DPACKAGE_NAME="${opt_package_name}" \
			-DPACKAGE_PREFIX="${opt_package_dir}" \
			-Dlibobs_DIR="${PWD}/third-party/obs-studio/build/install" \
			-DFFmpeg_DIR="${ffmpeg_dir}" \
			-DCURL_DIR="${curl_dir}" \
			-DQt6_DIR="${qt_dir}"
		if [[ $? != 0 ]]; then return $?; fi
	fi
	if [[ $? != 0 ]]; then return $?; fi
	group_end

	group_start "Build"
	cmake \
		--build build \
		--parallel \
		--config RelWithDebInfo \
		--target StreamFX
	if [[ $? != 0 ]]; then return $?; fi
	group_end

	group_start "Install"
	cmake \
		--install build \
		--config RelWithDebInfo
	if [[ $? != 0 ]]; then return $?; fi
	group_end
}

while [[ "$1" != "" ]]; do
	if [[ $(type -t "task_$1") == "function" ]]; then
		task="$1"
	elif [[ ("$1" == "--build") || ("$1" == "-b") ]]; then
		opt_build_dir="$2"
		shift
	elif [[ ("$1" == "--install") || ("$1" == "-i") ]]; then
		opt_install_dir="$2"
		shift
	elif [[ ("$1" == "--package") || ("$1" == "-p") ]]; then
		opt_package_dir="$2"
		shift
	elif [[ ("$1" == "--package-name") || ("$1" == "-n") ]]; then
		opt_package_name="$2"
		shift
	fi
	shift
done

if [[ $(type -t "task_${task}") == "function" ]]; then
	task_${task} $*
	exit $?
else
	echo "Usage: '$0' <command> <arguments>"
	echo "  Unknown command: $1"
	exit 1
fi

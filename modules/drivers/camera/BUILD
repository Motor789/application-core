load("//tools:apollo_package.bzl", "apollo_cc_binary", "apollo_cc_library", "apollo_cc_test", "apollo_component", "apollo_package")
load("//tools:cpplint.bzl", "cpplint")
load("//tools/platform:build_defs.bzl", "if_profiler")
load("//third_party/gpus:common.bzl", "gpu_library", "if_cuda", "if_rocm")


package(default_visibility = ["//visibility:public"])

CAMERA_COPTS = ['-DMODULE_NAME=\\"camera\\"']

filegroup(
    name = "camera_driver_files",
    srcs = glob([
        "conf/**",
        "dag/**",
        "data/**",
        "launch/**",
    ]),
)

gpu_library(
    name = "process_image_cuda",
    srcs = ["yuyv2rgb.cu"],
    hdrs = ["yuyv2rgb.h"],
    deps = 
       
        ["@local_config_cuda//cuda:cudart",]
)     

apollo_cc_library(
    name = "apollo_camera_util",
    srcs = select(
        {
            "@platforms//cpu:x86_64": [
                "usb_cam.cc",
                "util.cc",
            ],
            "@platforms//cpu:aarch64": [
                "usb_cam.cc",
            ],
        },
        no_match_error = "Please Build with an ARM or Linux x86_64 platform",
    ),
    hdrs = select(
        {
            "@platforms//cpu:x86_64": [
                "usb_cam.h",
                "util.h",
            ],
            "@platforms//cpu:aarch64": [
                "usb_cam.h",
            ],
        },
        no_match_error = "Please Build with an ARM or Linux x86_64 platform",
    ),
    copts = CAMERA_COPTS + if_profiler() + ["-DENABLE_PROFILER=1"],
    deps = [
        "@boost",
        "//cyber",
        "//modules/drivers/camera/proto:config_cc_proto",
        "@ffmpeg//:avcodec",
        ":process_image_cuda",
        "@local_config_cuda//cuda:cudart",   
        "//modules/perception/lidar_detection:point_pillars_common",
    ]
)

apollo_component(
    name = "libcamera_component.so",
    srcs = ["camera_component.cc"],
    hdrs = ["camera_component.h"],
    copts = CAMERA_COPTS,
    linkopts = [
        "-latomic",
    ],
    deps = [
        ":apollo_camera_util",
        "//cyber",
        "//modules/common_msgs/basic_msgs:error_code_cc_proto",
        "//modules/common_msgs/basic_msgs:header_cc_proto",
        "//modules/common_msgs/sensor_msgs:sensor_image_cc_proto",
        "@ffmpeg//:avcodec",
        "@ffmpeg//:avformat",
        "@ffmpeg//:swscale",
         
    ],
)

apollo_component(
    name = "libcamera_compress_component.so",
    srcs = ["compress_component.cc"],
    hdrs = ["compress_component.h"],
    copts = CAMERA_COPTS,
    deps = [
        "//cyber",
        "//modules/common_msgs/basic_msgs:error_code_cc_proto",
        "//modules/common_msgs/basic_msgs:header_cc_proto",
        "//modules/drivers/camera/proto:config_cc_proto",
        "//modules/common_msgs/sensor_msgs:sensor_image_cc_proto",
        "@opencv//:core",
        "@opencv//:highgui",
        "@opencv//:imgcodecs",
        "@opencv//:imgproc",
    ],
)


apollo_package()

cpplint()

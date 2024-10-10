load("//tools:cpplint.bzl", "cpplint")
load("//tools:apollo_package.bzl", "apollo_cc_binary", "apollo_cc_library", "apollo_cc_test", "apollo_component", "apollo_package")

package(default_visibility = ["//visibility:public"])

apollo_cc_library(
    name = "baidu_rtc_sdk",
    srcs = ["rtc/rtc_client.cc",],
    hdrs = [
    "rtc/rtc_client.h",
    "rtc/BaiduRtcInterface.h",
    "rtc/BaiduRtcCommonDefine.h",
    "rtc/BaiduExternalVideoRendererInterface.h",
    "rtc/BaiduExternalVideoCapturerInterface.h",
    "rtc/BaiduExternalDataRendererInterface.h",
    "rtc/BaiduExternalAudioRendererInterface.h"],
    deps = [
    
        "//cyber",
        "//modules/external_command/external_driver/proto:external_driver_config_cc_proto", 
    ],
)

apollo_component(
    name = "libexternal_driver_component.so",
    hdrs = ["external_driver_component.h"],
    srcs = ["external_driver_component.cc"],
    copts = ["-DMODULE_NAME=\\\"external_command\\\""],
    deps = [
        "//cyber",
        "//modules/common/util:message_util",
        "//modules/common_msgs/external_command_msgs:command_status_cc_proto",
        "//modules/common_msgs/external_command_msgs:action_command_cc_proto",
        "//modules/common_msgs/external_command_msgs:chassis_command_cc_proto",
        "//modules/common_msgs/external_command_msgs:free_space_command_cc_proto",
        "//modules/common_msgs/external_command_msgs:lane_follow_command_cc_proto",
        "//modules/common_msgs/external_command_msgs:path_follow_command_cc_proto",
        "//modules/common_msgs/external_command_msgs:speed_command_cc_proto",
        "//modules/common_msgs/external_command_msgs:valet_parking_command_cc_proto",
        "//modules/common_msgs/planning_msgs:planning_cc_proto",
        "//modules/external_command/external_driver/proto:sweeper_custom_command_cc_proto",
        "//modules/external_command/external_driver/proto:external_driver_config_cc_proto",
        "//modules/common_msgs/sensor_msgs:sensor_image_cc_proto",
        "@opencv//:imgproc",
        "@com_github_nlohmann_json//:json",
        ":baidu_rtc_sdk"

    ],
)

# apollo_component(
#     name = "libexternal_command_wrapper_demo_component.so",
#     hdrs = ["external_command_wrapper_demo.h"],
#     srcs = ["external_command_wrapper_demo.cc"],
#     copts = ["-DMODULE_NAME=\\\"external_command\\\""],
#     deps = [
#         "//cyber",
#         "//modules/common/util:message_util",
#         "//modules/common_msgs/external_command_msgs:command_status_cc_proto",
#         "//modules/common_msgs/external_command_msgs:action_command_cc_proto",
#         "//modules/common_msgs/external_command_msgs:chassis_command_cc_proto",
#         "//modules/common_msgs/external_command_msgs:free_space_command_cc_proto",
#         "//modules/common_msgs/external_command_msgs:lane_follow_command_cc_proto",
#         "//modules/common_msgs/external_command_msgs:path_follow_command_cc_proto",
#         "//modules/common_msgs/external_command_msgs:speed_command_cc_proto",
#         "//modules/common_msgs/external_command_msgs:valet_parking_command_cc_proto",
#         "//modules/common_msgs/planning_msgs:planning_cc_proto",
#         "//modules/common/service_wrapper:client_wrapper",
#         "//modules/external_command/external_driver/proto:sweeper_custom_command_cc_proto",
#         "//modules/external_command/external_driver/proto:external_driver_config_cc_proto",
#     ],
# )

filegroup(
    name = "conf",
    srcs = glob([
        "conf/*",
        "dag/*",
        "launch/*"
    ]),
)

cpplint()
apollo_package()

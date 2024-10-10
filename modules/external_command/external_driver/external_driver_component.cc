/******************************************************************************
 * Copyright 2023 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "modules/external_command/external_driver/external_driver_component.h"

#include <poll.h>

#include <cctype>

#include "modules/external_command/external_driver/proto/sweeper_custom_command.pb.h"

#include "cyber/common/file.h"
#include "cyber/record/record_reader.h"

#include "opencv2/opencv.hpp"
#include "nlohmann/json.hpp"
#include <fstream>

namespace apollo {
namespace external_command {
using apollo::external_command::CommandStatus;

// ExternalDriver::ExternalDriver() : command_id_(0), module_name_("demo") {}

bool ExternalDriver::Init() {
    action_command_client_ = node_->CreateClient<apollo::external_command::ActionCommand, CommandStatus>(
            "/apollo/external_command/action");
    chassis_command_client_ = node_->CreateClient<apollo::external_command::ChassisCommand, CommandStatus>(
            "/apollo/external_command/chassis");
    free_space_command_client_ = node_->CreateClient<apollo::external_command::FreeSpaceCommand, CommandStatus>(
            "/apollo/external_command/free_space");
    lane_follow_command_client_ = node_->CreateClient<apollo::external_command::LaneFollowCommand, CommandStatus>(
            "/apollo/external_command/lane_follow");
    path_follow_command_client_ = node_->CreateClient<apollo::external_command::PathFollowCommand, CommandStatus>(
            "/apollo/external_command/path_follow");
    speed_command_client_ = node_->CreateClient<apollo::external_command::SpeedCommand, CommandStatus>(
            "/apollo/external_command/speed");
    valet_parking_command_client_ = node_->CreateClient<apollo::external_command::ValetParkingCommand, CommandStatus>(
            "/apollo/external_command/valet_parking");
    status_client_ = node_->CreateClient<apollo::external_command::CommandStatusRequest, CommandStatus>(
            "/apollo/external_command/command_status");
    apollo::cyber::common::GetProtoFromFile(
            "/apollo/modules/external_command/external_driver/conf/"
            "external_driver_config.pb.txt",
            &config_);
    // reader_ = node_->CreateReader<apollo::drivers::Image>("/apollo/sensor/camera/front_6mm/image", nullptr);

    rtc_client_.CreateClient(config_);

    std::ifstream f(config_.destination_path());
    if (f.fail()) {
        AERROR << "failed to load destination file";
    }
    nlohmann::json endpoint = nlohmann::json::parse(f);
    destination = endpoint.dump();

    return true;
}

void ExternalDriver::InternalProc() {
    // reader_->Observe();
    // const auto image_msg = reader_->GetLatestObserved();
    // ProcessImage(image_msg);
    AINFO<< "msg type:" << rtc_client_.g_mylistener.msg_type;
    if (rtc_client_.g_mylistener.msg_type != 301 && rtc_client_.g_mylistener.msg_type != 2000) {
        reader_ = node_->CreateReader<apollo::drivers::Image>(
                "/apollo/sensor/camera/front_6mm/image",
                [&](const std::shared_ptr<apollo::drivers::Image>& image) { ProcessImage(image); });
        AINFO << "start to read image and send to rtc cloud";
    }
}

bool ExternalDriver::ProcessImage(const std::shared_ptr<apollo::drivers::Image>& image) {
    if (image == nullptr) {
        return false;
    }
    cv::Mat img_front(image->height(), image->width(), CV_8UC3, const_cast<char*>(image->data().data()));
    cv::cvtColor(img_front, img_front, cv::COLOR_RGB2BGR);
    std::vector<unsigned char> buf;
    cv::imencode(".jpg", img_front, buf);
    // int n = 0;
    // n++;
    // if (n<5){
    //     rtc_client_.g_BrtcClient->sendMessageToUser(destination.c_str(),id.c_str());
    // }

    rtc_client_.g_BrtcClient->sendImage(reinterpret_cast<const char*>(buf.data()), buf.size());
    return true;
}
bool ExternalDriver::Proc() {
    // int8_t revent = 0;  // short
    // struct pollfd fd = {STDIN_FILENO, POLLIN, revent};
    // switch (poll(&fd, 1, 100)) {
    // case -1:
    //     AINFO << "Failed to read keyboard" ;
    //     return false;
    // case 0:
    //     return true;
    // default:
    // char data[50];
    // std::cin.getline(data, 50);
    InternalProc();

    std::string data = rtc_client_.g_mylistener.recieve_msg;
    int msgtype = rtc_client_.g_mylistener.msg_type;
    std::string input_command_string = "";
    if (!data.empty() && rtc_client_.g_mylistener.re_mark) {
        const nlohmann::json command = nlohmann::json::parse(data);
        input_command_string = command["action"];
    }

    if (input_command_string == "pull_over") {
        SendActionCommand(apollo::external_command::ActionCommandType::PULL_OVER);
    } else if (input_command_string == "stop") {
        // Stop planning.
        SendActionCommand(apollo::external_command::ActionCommandType::STOP);
    } else if (input_command_string == "start") {
        // Start planning.
        SendActionCommand(apollo::external_command::ActionCommandType::START);
    } else if (input_command_string == "clear") {
        // Start planning.
        SendActionCommand(apollo::external_command::ActionCommandType::CLEAR_PLANNING);
    } else if (input_command_string == "manual") {
        // Switch manual mode.
        SendActionCommand(apollo::external_command::ActionCommandType::SWITCH_TO_MANUAL);
    } else if (input_command_string == "auto") {
        // Switch auto mode.
        SendActionCommand(apollo::external_command::ActionCommandType::SWITCH_TO_AUTO);
    } else if (input_command_string == "vin") {
        // Send vin validation.
        SendActionCommand(apollo::external_command::ActionCommandType::VIN_REQ);
    } else if (input_command_string == "enter_mission") {
        // Enter mission model.
        SendActionCommand(apollo::external_command::ActionCommandType::ENTER_MISSION);
    } else if (input_command_string == "exit_mission") {
        // Exit mission model.
        SendActionCommand(apollo::external_command::ActionCommandType::EXIT_MISSION);
    } else if (input_command_string == "chassis") {
        SendVehicleSignalCommand();
    } else if (input_command_string == "custom_chassis") {
        SendCustomChassisCommand();
    } else if (input_command_string.find("set_speed") != std::string::npos) {
        auto index = input_command_string.find("set_speed");
        std::string speed_value_string
                = input_command_string.substr(index + std::string("set_speed").length(), input_command_string.length());
        if (!speed_value_string.empty()) {
            double speed_value = std::atof(speed_value_string.c_str());
            SendSpeedCommand(speed_value);
        } else {
            AWARN << "Input format is invalid, please input format like: "
                     "set_speed 1.5";
        }
    } else if (input_command_string == "increase_speed") {
        double speed_factor = 1.2;
        SendSpeedFactorCommand(speed_factor);
    } else if (input_command_string == "decrease_speed") {
        double speed_factor = 0.8;
        SendSpeedFactorCommand(speed_factor);
    } else if (input_command_string == "restore_speed") {
        RestoreSpeed();
    } else if (input_command_string == "lane") {
        // Modify way point as needed.
        apollo::external_command::Pose way_point;
        way_point.set_x(config_.point1_x());
        way_point.set_y(config_.point1_y());
        way_point.set_heading(0.0);
        std::vector<apollo::external_command::Pose> way_points;
        way_points.emplace_back(way_point);
        apollo::external_command::Pose end_pose;
        end_pose.set_x(config_.end_pose_x());
        end_pose.set_y(config_.end_pose_y());
        end_pose.set_heading(config_.end_pose_heading());
        SendLaneFollowCommand(way_points, end_pose, config_.target_speed());
    } else if (input_command_string == "path_loc") {
        SendPathFollowCommandWithLocationRecord(config_.file_of_path_follow_with_localization_record());
    } else if (input_command_string == "path_path") {
        SendPathFollowCommandWithPathRecord(config_.file_of_path_follow_with_planning_record());
    } else if (input_command_string == "valet_parking") {
        std::string parking_spot_id = "ParkingSpace_1";
        SendValetParkingCommand(parking_spot_id, config_.target_speed());
    } else if (input_command_string.find("command_status") != std::string::npos) {
        // Input command with format: command_status=XX, where XX is the
        // command_id
        const size_t start_pos = std::string("command_status=").length();
        const size_t end_pos = input_command_string.length();
        if (end_pos <= start_pos) {
            AINFO << "Please check command status with format: command_status=XX!" ;
        }
        std::string command_id_string = input_command_string.substr(start_pos, end_pos);
        command_id_string.erase(
                std::remove_if(command_id_string.begin(), command_id_string.end(), ::isspace), command_id_string.end());
        uint64_t id = std::atoi(command_id_string.c_str());
        CheckCommandStatus(id);
    } else if (input_command_string == "free1") {
        apollo::external_command::Pose end_pose;
        end_pose.set_x(437556.02);
        end_pose.set_y(4432540.34);
        end_pose.set_heading(1.8);
        std::vector<apollo::external_command::Point> way_points;
        apollo::external_command::Point point1;
        apollo::external_command::Point point2;
        apollo::external_command::Point point3;
        apollo::external_command::Point point4;
        point1.set_x(437536.29);
        point1.set_y(4432560.69);
        point2.set_x(437536.29);
        point2.set_y(4432510.69);
        point3.set_x(437576.29);
        point3.set_y(4432510.69);
        point4.set_x(437576.29);
        point4.set_y(4432560.69);
        way_points.emplace_back(point1);
        way_points.emplace_back(point2);
        way_points.emplace_back(point3);
        way_points.emplace_back(point4);

        SendFreespaceCommand(way_points, end_pose);
    } else {
        AINFO << "Invalid input!" << input_command_string;
    }
    rtc_client_.g_mylistener.re_mark = false;
    return true;
}

void ExternalDriver::SendActionCommand(apollo::external_command::ActionCommandType action_command_type) {
    auto command = std::make_shared<apollo::external_command::ActionCommand>();
    FillCommandHeader(command);
    command->set_command(action_command_type);
    AINFO << "Sending action command: " << command->DebugString() ;
    auto response = action_command_client_->SendRequest(command);
    if (nullptr == response) {
        AERROR << "Command sending failed, please check the service is on!\n";
    } else {
        AINFO << "******Finish sending command.******\n";
    }
}

void ExternalDriver::SendVehicleSignalCommand() {
    // Send left turn signal.
    auto command = std::make_shared<apollo::external_command::ChassisCommand>();
    FillCommandHeader(command);
    command->mutable_basic_signal()->set_turn_signal(apollo::common::VehicleSignal::TURN_LEFT);
    AINFO << "Sending chassis command: " << command->DebugString() ;
    auto response = chassis_command_client_->SendRequest(command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}

void ExternalDriver::SendCustomChassisCommand() {
    // Send left turn signal.
    auto command = std::make_shared<apollo::external_command::ChassisCommand>();
    FillCommandHeader(command);
    // Set custom command.
    auto custom_operation = command->mutable_custom_operation();
    // Set custom command values.
    apollo::external_command::SweeperCustomCommand sweeper_command;
    sweeper_command.set_is_turn_on_brush(true);
    sweeper_command.set_sweeping_speed(2.0);
    custom_operation->PackFrom(sweeper_command);
    AINFO << "Sending custom chassis command: " << command->DebugString() ;
    auto response = chassis_command_client_->SendRequest(command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}

void ExternalDriver::SendPathFollowCommandWithPathRecord(const std::string& record_path) {
    // Read planning data from record file and use the planning path points
    // as the path of PathFollowCommand.
    apollo::planning::ADCTrajectory record_planning_data;
    // Get the path points from record planning data.
    std::shared_ptr<apollo::external_command::PathFollowCommand> path_follow_command
            = std::make_shared<apollo::external_command::PathFollowCommand>();
    ReadPathFromPathRecord(record_path, path_follow_command->mutable_way_point());
    // Set header and command id of PathFollowCommand.
    FillCommandHeader(path_follow_command);
    // Set path boundary of path.
    auto path_boundary = path_follow_command->mutable_boundary_with_width();
    path_boundary->set_left_path_width(config_.left_path_width());
    path_boundary->set_right_path_width(config_.right_path_width());
    // Set target speed.
    path_follow_command->set_target_speed(config_.target_speed());
    auto response = path_follow_command_client_->SendRequest(path_follow_command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}

void ExternalDriver::SendPathFollowCommandWithLocationRecord(const std::string& record_dir) {
    // Read planning data from record file and use the planning path points
    // as the path of PathFollowCommand.
    std::shared_ptr<apollo::external_command::PathFollowCommand> path_follow_command
            = std::make_shared<apollo::external_command::PathFollowCommand>();
    std::vector<std::string> record_files = apollo::cyber::common::ListSubPaths(record_dir, DT_REG);
    std::string dir_prefix = record_dir + '/';
    for (const auto file_name : record_files) {
        ReadPathFromLocationRecord(dir_prefix + file_name, path_follow_command->mutable_way_point());
    }

    // Set header and command id of PathFollowCommand.
    FillCommandHeader(path_follow_command);
    // Set path boundary of path.
    auto path_boundary = path_follow_command->mutable_boundary_with_width();
    path_boundary->set_left_path_width(config_.left_path_width());
    path_boundary->set_right_path_width(config_.right_path_width());
    // Set target speed.
    path_follow_command->set_target_speed(config_.target_speed());
    auto response = path_follow_command_client_->SendRequest(path_follow_command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}

void ExternalDriver::SendSpeedCommand(double speed) {
    auto command = std::make_shared<apollo::external_command::SpeedCommand>();
    FillCommandHeader(command);
    command->set_target_speed(speed);
    AINFO << "Sending speed command: " << command->DebugString() ;
    auto response = speed_command_client_->SendRequest(command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}

void ExternalDriver::SendSpeedFactorCommand(double speed_factor) {
    auto command = std::make_shared<apollo::external_command::SpeedCommand>();
    FillCommandHeader(command);
    command->set_target_speed_factor(speed_factor);
    AINFO << "Sending speed factor command: " << command->DebugString() ;
    auto response = speed_command_client_->SendRequest(command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}

void ExternalDriver::RestoreSpeed() {
    auto command = std::make_shared<apollo::external_command::SpeedCommand>();
    FillCommandHeader(command);
    command->set_is_restore_target_speed(true);
    AINFO << "Restore speed: " << command->DebugString() ;
    auto response = speed_command_client_->SendRequest(command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}

void ExternalDriver::SendLaneFollowCommand(
        const std::vector<apollo::external_command::Pose>& way_points,
        const apollo::external_command::Pose& end,
        double target_speed) {
    auto command = std::make_shared<apollo::external_command::LaneFollowCommand>();
    FillCommandHeader(command);
    // Copy way_points
    // for (const auto& point : way_points) {
    //     auto way_point = command->add_way_point();
    //     way_point->CopyFrom(point);
    // }
    // Copy end point
    command->mutable_end_pose()->CopyFrom(end);
    if (target_speed > 0.0) {
        command->set_target_speed(target_speed);
    }
    // command->set_is_start_pose_set(true);
    AINFO << "Sending lane follow command: " << command->DebugString() ;
    auto response = lane_follow_command_client_->SendRequest(command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}

void ExternalDriver::SendFreespaceCommand(
        const std::vector<apollo::external_command::Point>& way_points,
        const apollo::external_command::Pose& end) {
    auto command = std::make_shared<apollo::external_command::FreeSpaceCommand>();
    FillCommandHeader(command);
    // Copy way_points
    auto roi_point = command->mutable_drivable_roi();
    for (const auto& point : way_points) {
        roi_point->add_point()->CopyFrom(point);
    }
    // Copy end point
    command->mutable_parking_spot_pose()->CopyFrom(end);
    AINFO << "Sending lane follow command: " << command->DebugString() ;
    auto response = free_space_command_client_->SendRequest(command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}
void ExternalDriver::SendValetParkingCommand(const std::string& parking_spot_id, double target_speed) {
    auto command = std::make_shared<apollo::external_command::ValetParkingCommand>();
    FillCommandHeader(command);
    command->set_parking_spot_id(parking_spot_id);
    if (target_speed > 0.0) {
        command->set_target_speed(target_speed);
    }
    AINFO << "Sending valet parking command: " << command->DebugString() ;
    auto response = valet_parking_command_client_->SendRequest(command);
    if (nullptr == response) {
        AINFO << "Command sending failed, please check the service is on!\n" ;
    } else {
        AINFO << "******Finish sending command.******\n" ;
    }
}

void ExternalDriver::ReadPathFromPathRecord(
        const std::string& record_file,
        google::protobuf::RepeatedPtrField<apollo::external_command::Point>* waypoints) {
    AINFO << "ReadPathFromPathRecord: " << record_file ;
    apollo::cyber::record::RecordReader reader(record_file);
    if (!reader.IsValid()) {
        AINFO << "Fail to open " << record_file ;
        return;
    }

    apollo::planning::ADCTrajectory planning_trajectory;
    apollo::cyber::record::RecordMessage message;
    while (reader.ReadMessage(&message)) {
        if (message.channel_name == "/apollo/planning") {
            if (planning_trajectory.ParseFromString(message.content)) {
                break;
            }
        }
    }
    waypoints->Clear();
    const auto& trajectory_points = planning_trajectory.trajectory_point();
    for (const auto input_point : trajectory_points) {
        auto output_point = waypoints->Add();
        output_point->set_x(input_point.path_point().x());
        output_point->set_y(input_point.path_point().y());
    }
}

void ExternalDriver::CheckCommandStatus(const uint64_t command_id) {
    auto command = std::make_shared<apollo::external_command::CommandStatusRequest>();
    FillCommandHeader(command);
    command->set_command_id(command_id);
    AINFO << "Sending check command: " << command->DebugString() ;
    auto response = status_client_->SendRequest(command);
    if (nullptr == response) {
        AINFO << "Check status failed!\n" ;
    } else {
        AINFO << response->DebugString();
        AINFO << "******Finish checking command.******\n" ;
    }
}

void ExternalDriver::ReadPathFromLocationRecord(
        const std::string& record_file,
        google::protobuf::RepeatedPtrField<apollo::external_command::Point>* waypoints) const {
    AINFO << "ReadPathFromLocationRecord: " << record_file ;
    apollo::cyber::record::RecordReader reader(record_file);
    if (!reader.IsValid()) {
        AINFO << "Fail to open " << record_file ;
        return;
    }
    apollo::localization::LocalizationEstimate localization;
    apollo::cyber::record::RecordMessage message;
    double last_x = 0.0;
    double last_y = 0.0;
    bool is_last_poistion_set = false;
    while (reader.ReadMessage(&message)) {
        if (message.channel_name == "/apollo/localization/pose") {
            if (localization.ParseFromString(message.content)) {
                const auto& position = localization.pose().position();
                if (!is_last_poistion_set) {
                    last_x = position.x();
                    last_y = position.y();
                    is_last_poistion_set = true;
                    continue;
                }
                // Save the location to path with min_distance.
                if (abs(last_x - position.x()) > config_.min_distance_error()
                    || abs(last_y - position.y()) > config_.min_distance_error()) {
                    auto output_point = waypoints->Add();
                    output_point->set_x(position.x());
                    output_point->set_y(position.y());
                    last_x = position.x();
                    last_y = position.y();
                }
            }
        }
    }
}
}  // namespace external_command
}  // namespace apollo
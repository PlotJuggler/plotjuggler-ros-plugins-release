#pragma once

#include <nav_msgs/Odometry.h>
#include "pose_msg.h"
#include "twist_msg.h"
#include "ros1_parser.h"
#include "header_msg.h"

class OdometryMsgParser : public BuiltinMessageParser<nav_msgs::Odometry>
{
public:
  OdometryMsgParser(const std::string& topic_name, PJ::PlotDataMapRef& plot_data)
    : BuiltinMessageParser<nav_msgs::Odometry>(topic_name, plot_data)
    , _header_parser(topic_name + "/header", plot_data)
    , _pose_parser(topic_name + "/pose", plot_data)
    , _twist_parser(topic_name + "/twist", plot_data)
  {
  }

  void parseMessageImpl(const nav_msgs::Odometry& msg, double& timestamp) override
  {
    _header_parser.parse(msg.header, timestamp, _use_header_stamp);
    _pose_parser.parseMessageImpl(msg.pose, timestamp);
    _twist_parser.parseMessageImpl(msg.twist, timestamp);
  }

private:
  HeaderMsgParser _header_parser;
  PoseCovarianceMsgParser _pose_parser;
  TwistCovarianceMsgParser _twist_parser;
};

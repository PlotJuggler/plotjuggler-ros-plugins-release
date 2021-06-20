#pragma once

#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <geometry_msgs/msg/twist_with_covariance.hpp>
#include "quaternion_msg.h"
#include "ros2_parser.h"
#include "header_msg.h"

class TwistMsgParser : public BuiltinMessageParser<geometry_msgs::msg::Twist>
{
 public:
  TwistMsgParser(const std::string& topic_name, PJ::PlotDataMapRef& plot_data)
      : BuiltinMessageParser<geometry_msgs::msg::Twist>(topic_name, plot_data)
  {
  }

  void parseMessageImpl(const geometry_msgs::msg::Twist& msg, double& timestamp) override
  {
    if( !_initialized )
    {
      _initialized = true;
      _data.push_back(&getSeries(_topic_name + "/linear/x"));
      _data.push_back(&getSeries(_topic_name + "/linear/y"));
      _data.push_back(&getSeries(_topic_name + "/linear/z"));

      _data.push_back(&getSeries(_topic_name + "/angular/x"));
      _data.push_back(&getSeries(_topic_name + "/angular/y"));
      _data.push_back(&getSeries(_topic_name + "/angular/z"));
    }

    _data[0]->pushBack({ timestamp, msg.linear.x });
    _data[1]->pushBack({ timestamp, msg.linear.y });
    _data[2]->pushBack({ timestamp, msg.linear.z });

    _data[3]->pushBack({ timestamp, msg.angular.x });
    _data[4]->pushBack({ timestamp, msg.angular.y });
    _data[5]->pushBack({ timestamp, msg.angular.z });
  }

 private:
  std::vector<PJ::PlotData*> _data;
  bool _initialized = false;
};

class TwistStampedMsgParser : public BuiltinMessageParser<geometry_msgs::msg::TwistStamped>
{
 public:
  TwistStampedMsgParser(const std::string& topic_name, PJ::PlotDataMapRef& plot_data)
      : BuiltinMessageParser<geometry_msgs::msg::TwistStamped>(topic_name, plot_data)
        , _header_parser(topic_name + "/header", plot_data)
        , _twist_parser(topic_name + "/twist", plot_data)
  {
  }

  void parseMessageImpl(const geometry_msgs::msg::TwistStamped& msg, double& timestamp) override
  {
    _header_parser.parse(msg.header, timestamp, _use_header_stamp);
    _twist_parser.parseMessageImpl(msg.twist, timestamp);
  }

 private:
  HeaderMsgParser _header_parser;
  TwistMsgParser _twist_parser;
};

class TwistCovarianceMsgParser : public BuiltinMessageParser<geometry_msgs::msg::TwistWithCovariance>
{
 public:
  TwistCovarianceMsgParser(const std::string& topic_name, PJ::PlotDataMapRef& plot_data)
      : BuiltinMessageParser<geometry_msgs::msg::TwistWithCovariance>(topic_name, plot_data)
        , _twist_parser(topic_name + "/twist", plot_data)
        , _covariance(topic_name + "/covariance", plot_data)
  {
  }

  void parseMessageImpl(const geometry_msgs::msg::TwistWithCovariance& msg, double& timestamp) override
  {
    _twist_parser.parseMessageImpl(msg.twist, timestamp);
    _covariance.parse(msg.covariance, timestamp);
  }

 private:
  TwistMsgParser _twist_parser;
  CovarianceParser<6> _covariance;
};

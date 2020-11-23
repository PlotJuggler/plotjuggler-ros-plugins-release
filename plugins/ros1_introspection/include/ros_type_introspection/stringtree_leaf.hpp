/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright 2016-2017 Davide Faconti
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
* *******************************************************************/

#ifndef ROS_INTROSPECTION_STRINGTREELEAF_H
#define ROS_INTROSPECTION_STRINGTREELEAF_H

#include <vector>
#include <map>
#include <iostream>
#include <boost/container/small_vector.hpp>
#include <boost/container/static_vector.hpp>
#include "ros_type_introspection/ros_message.hpp"

namespace RosIntrospection{

/**
 * @brief The StringTreeLeaf is, as the name suggests, a leaf (terminal node)
 * of a StringTree.
 * It provides the pointer to the node and a list of numbers that represent
 * the index that corresponds to the placeholder "#".
 *
 * For example if you want to represent the string
 *
 *      foo/2/bar/3/hello/world
 *
 * This would correspond to a branch of the tree (from root to the leaf) equal to these 6 nodes,
 * where "foo" is the root and "world" is the leaf
 *
 * foo -> # -> bar -> # ->hello -> world
 *
 * array_size will be equal to two and index_array will contain these numbers {2,3}
 *
 */
struct StringTreeLeaf{

  StringTreeLeaf();

  const StringTreeNode* node_ptr;

  boost::container::static_vector<uint16_t,8> index_array;

  /// Utility functions to print the entire branch
  bool toStr(std::string &destination) const;

  // return string length or -1 if failed
  int toStr(char* buffer) const;

  std::string toStdString() const { std::string out; toStr(out); return out; }

  constexpr static const char SEPARATOR = '/';
  constexpr static const char NUM_PLACEHOLDER = '#';

  static const boost::string_ref& num_placeholder() {
    static const boost::string_ref nph("#");
    return nph;
  }
};

void CreateStringFromTreeLeaf(const StringTreeLeaf& leaf, bool skip_root, std::string &out);

//---------------------------------

inline std::ostream& operator<<(std::ostream &os, const StringTreeLeaf& leaf )
{
  std::string dest;
  leaf.toStr(dest);
  os << dest;
  return os;
}

inline StringTreeLeaf::StringTreeLeaf(): node_ptr(nullptr)
{  }


inline bool StringTreeLeaf::toStr(std::string& destination) const
{
  char buffer[256];
  int offset = this->toStr(buffer);

  if( offset < 0 ) {
    destination.clear();
    return false;
  }
  destination.assign(buffer, offset);
  return true;
}


}

#endif // ROSTYPE_H

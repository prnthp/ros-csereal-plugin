#ifndef _ROS_osiris_Bone_h
#define _ROS_osiris_Bone_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/Pose.h"

namespace osiris
{

  class Bone : public ros::Msg
  {
    public:
      typedef const char* _bone_name_type;
      _bone_name_type bone_name;
      typedef double _finger_width_type;
      _finger_width_type finger_width;
      typedef geometry_msgs::Pose _pose_type;
      _pose_type pose;

    Bone():
      bone_name(""),
      finger_width(0),
      pose()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_bone_name = strlen(this->bone_name);
      varToArr(outbuffer + offset, length_bone_name);
      offset += 4;
      memcpy(outbuffer + offset, this->bone_name, length_bone_name);
      offset += length_bone_name;
      union {
        double real;
        uint64_t base;
      } u_finger_width;
      u_finger_width.real = this->finger_width;
      *(outbuffer + offset + 0) = (u_finger_width.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_finger_width.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_finger_width.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_finger_width.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_finger_width.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_finger_width.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_finger_width.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_finger_width.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->finger_width);
      offset += this->pose.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_bone_name;
      arrToVar(length_bone_name, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_bone_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_bone_name-1]=0;
      this->bone_name = (char *)(inbuffer + offset-1);
      offset += length_bone_name;
      union {
        double real;
        uint64_t base;
      } u_finger_width;
      u_finger_width.base = 0;
      u_finger_width.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_finger_width.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_finger_width.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_finger_width.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_finger_width.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_finger_width.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_finger_width.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_finger_width.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->finger_width = u_finger_width.real;
      offset += sizeof(this->finger_width);
      offset += this->pose.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "osiris/Bone"; };
    const char * getMD5(){ return "b9150a3ece66b9548d3d365ca8676cd2"; };

  };

}
#endif
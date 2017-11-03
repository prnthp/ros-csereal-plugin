#ifndef _ROS_osiris_Values_h
#define _ROS_osiris_Values_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace osiris
{

  class Values : public ros::Msg
  {
    public:
      typedef const char* _name_type;
      _name_type name;
      typedef double _data_type;
      _data_type data;

    Values():
      name(""),
      data(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_name = strlen(this->name);
      varToArr(outbuffer + offset, length_name);
      offset += 4;
      memcpy(outbuffer + offset, this->name, length_name);
      offset += length_name;
      union {
        double real;
        uint64_t base;
      } u_data;
      u_data.real = this->data;
      *(outbuffer + offset + 0) = (u_data.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_data.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_data.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_data.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_data.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_data.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_data.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_data.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->data);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_name;
      arrToVar(length_name, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_name-1]=0;
      this->name = (char *)(inbuffer + offset-1);
      offset += length_name;
      union {
        double real;
        uint64_t base;
      } u_data;
      u_data.base = 0;
      u_data.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_data.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_data.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_data.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_data.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_data.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_data.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_data.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->data = u_data.real;
      offset += sizeof(this->data);
     return offset;
    }

    const char * getType(){ return "osiris/Values"; };
    const char * getMD5(){ return "c62b160c39494f9cfcbbd7a0e565a3eb"; };

  };

}
#endif
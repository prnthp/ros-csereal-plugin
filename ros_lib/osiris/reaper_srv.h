#ifndef _ROS_SERVICE_reaper_srv_h
#define _ROS_SERVICE_reaper_srv_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace osiris
{

static const char REAPER_SRV[] = "osiris/reaper_srv";

  class reaper_srvRequest : public ros::Msg
  {
    public:
      typedef const char* _input_type;
      _input_type input;

    reaper_srvRequest():
      input("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_input = strlen(this->input);
      varToArr(outbuffer + offset, length_input);
      offset += 4;
      memcpy(outbuffer + offset, this->input, length_input);
      offset += length_input;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_input;
      arrToVar(length_input, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_input; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_input-1]=0;
      this->input = (char *)(inbuffer + offset-1);
      offset += length_input;
     return offset;
    }

    const char * getType(){ return REAPER_SRV; };
    const char * getMD5(){ return "39e92f1778057359c64c7b8a7d7b19de"; };

  };

  class reaper_srvResponse : public ros::Msg
  {
    public:
      typedef int64_t _success_type;
      _success_type success;

    reaper_srvResponse():
      success(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int64_t real;
        uint64_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_success.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_success.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_success.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_success.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_success.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_success.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_success.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->success);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int64_t real;
        uint64_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_success.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_success.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_success.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_success.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_success.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_success.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_success.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->success = u_success.real;
      offset += sizeof(this->success);
     return offset;
    }

    const char * getType(){ return REAPER_SRV; };
    const char * getMD5(){ return "0f26af2399fbfbc8170d9a60ed7b9846"; };

  };

  class reaper_srv {
    public:
    typedef reaper_srvRequest Request;
    typedef reaper_srvResponse Response;
  };

}
#endif

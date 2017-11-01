#ifndef _ROS_SERVICE_ganesh_srv_h
#define _ROS_SERVICE_ganesh_srv_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace osiris
{

static const char GANESH_SRV[] = "osiris/ganesh_srv";

  class ganesh_srvRequest : public ros::Msg
  {
    public:
      typedef const char* _command_type;
      _command_type command;
      typedef const char* _filename_type;
      _filename_type filename;
      typedef const char* _topics_type;
      _topics_type topics;

    ganesh_srvRequest():
      command(""),
      filename(""),
      topics("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_command = strlen(this->command);
      varToArr(outbuffer + offset, length_command);
      offset += 4;
      memcpy(outbuffer + offset, this->command, length_command);
      offset += length_command;
      uint32_t length_filename = strlen(this->filename);
      varToArr(outbuffer + offset, length_filename);
      offset += 4;
      memcpy(outbuffer + offset, this->filename, length_filename);
      offset += length_filename;
      uint32_t length_topics = strlen(this->topics);
      varToArr(outbuffer + offset, length_topics);
      offset += 4;
      memcpy(outbuffer + offset, this->topics, length_topics);
      offset += length_topics;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_command;
      arrToVar(length_command, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_command; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_command-1]=0;
      this->command = (char *)(inbuffer + offset-1);
      offset += length_command;
      uint32_t length_filename;
      arrToVar(length_filename, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_filename; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_filename-1]=0;
      this->filename = (char *)(inbuffer + offset-1);
      offset += length_filename;
      uint32_t length_topics;
      arrToVar(length_topics, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_topics; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_topics-1]=0;
      this->topics = (char *)(inbuffer + offset-1);
      offset += length_topics;
     return offset;
    }

    const char * getType(){ return GANESH_SRV; };
    const char * getMD5(){ return "b0321a84e7d9658d52e2ead237da2ccd"; };

  };

  class ganesh_srvResponse : public ros::Msg
  {
    public:
      typedef int64_t _success_type;
      _success_type success;
      typedef const char* _bag_output_type;
      _bag_output_type bag_output;

    ganesh_srvResponse():
      success(0),
      bag_output("")
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
      uint32_t length_bag_output = strlen(this->bag_output);
      varToArr(outbuffer + offset, length_bag_output);
      offset += 4;
      memcpy(outbuffer + offset, this->bag_output, length_bag_output);
      offset += length_bag_output;
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
      uint32_t length_bag_output;
      arrToVar(length_bag_output, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_bag_output; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_bag_output-1]=0;
      this->bag_output = (char *)(inbuffer + offset-1);
      offset += length_bag_output;
     return offset;
    }

    const char * getType(){ return GANESH_SRV; };
    const char * getMD5(){ return "eb9a2fed00364e35cecd2a6a37fb3993"; };

  };

  class ganesh_srv {
    public:
    typedef ganesh_srvRequest Request;
    typedef ganesh_srvResponse Response;
  };

}
#endif

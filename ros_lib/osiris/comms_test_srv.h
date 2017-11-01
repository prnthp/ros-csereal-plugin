#ifndef _ROS_SERVICE_comms_test_srv_h
#define _ROS_SERVICE_comms_test_srv_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace osiris
{

static const char COMMS_TEST_SRV[] = "osiris/comms_test_srv";

  class comms_test_srvRequest : public ros::Msg
  {
    public:
      typedef const char* _input_type;
      _input_type input;

    comms_test_srvRequest():
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

    const char * getType(){ return COMMS_TEST_SRV; };
    const char * getMD5(){ return "39e92f1778057359c64c7b8a7d7b19de"; };

  };

  class comms_test_srvResponse : public ros::Msg
  {
    public:
      typedef const char* _hash_type;
      _hash_type hash;

    comms_test_srvResponse():
      hash("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_hash = strlen(this->hash);
      varToArr(outbuffer + offset, length_hash);
      offset += 4;
      memcpy(outbuffer + offset, this->hash, length_hash);
      offset += length_hash;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_hash;
      arrToVar(length_hash, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_hash; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_hash-1]=0;
      this->hash = (char *)(inbuffer + offset-1);
      offset += length_hash;
     return offset;
    }

    const char * getType(){ return COMMS_TEST_SRV; };
    const char * getMD5(){ return "1a7b3248d0d2dd282eced94c9859f571"; };

  };

  class comms_test_srv {
    public:
    typedef comms_test_srvRequest Request;
    typedef comms_test_srvResponse Response;
  };

}
#endif

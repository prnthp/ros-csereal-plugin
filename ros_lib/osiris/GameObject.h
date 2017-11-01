#ifndef _ROS_osiris_GameObject_h
#define _ROS_osiris_GameObject_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/Pose.h"

namespace osiris
{

  class GameObject : public ros::Msg
  {
    public:
      typedef int32_t _unique_id_type;
      _unique_id_type unique_id;
      typedef int32_t _frame_count_type;
      _frame_count_type frame_count;
      typedef double _time_type;
      _time_type time;
      typedef const char* _parent_type;
      _parent_type parent;
      typedef int32_t _num_poses_type;
      _num_poses_type num_poses;
      uint32_t poses_length;
      typedef geometry_msgs::Pose _poses_type;
      _poses_type st_poses;
      _poses_type * poses;
      typedef bool _has_event_type;
      _has_event_type has_event;
      typedef const char* _events_type;
      _events_type events;
      typedef bool _has_values_type;
      _has_values_type has_values;
      uint32_t values_length;
      typedef double _values_type;
      _values_type st_values;
      _values_type * values;

    GameObject():
      unique_id(0),
      frame_count(0),
      time(0),
      parent(""),
      num_poses(0),
      poses_length(0), poses(NULL),
      has_event(0),
      events(""),
      has_values(0),
      values_length(0), values(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_unique_id;
      u_unique_id.real = this->unique_id;
      *(outbuffer + offset + 0) = (u_unique_id.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_unique_id.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_unique_id.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_unique_id.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->unique_id);
      union {
        int32_t real;
        uint32_t base;
      } u_frame_count;
      u_frame_count.real = this->frame_count;
      *(outbuffer + offset + 0) = (u_frame_count.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_frame_count.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_frame_count.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_frame_count.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->frame_count);
      union {
        double real;
        uint64_t base;
      } u_time;
      u_time.real = this->time;
      *(outbuffer + offset + 0) = (u_time.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_time.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_time.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_time.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_time.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_time.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_time.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_time.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->time);
      uint32_t length_parent = strlen(this->parent);
      varToArr(outbuffer + offset, length_parent);
      offset += 4;
      memcpy(outbuffer + offset, this->parent, length_parent);
      offset += length_parent;
      union {
        int32_t real;
        uint32_t base;
      } u_num_poses;
      u_num_poses.real = this->num_poses;
      *(outbuffer + offset + 0) = (u_num_poses.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_num_poses.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_num_poses.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_num_poses.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->num_poses);
      *(outbuffer + offset + 0) = (this->poses_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->poses_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->poses_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->poses_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->poses_length);
      for( uint32_t i = 0; i < poses_length; i++){
      offset += this->poses[i].serialize(outbuffer + offset);
      }
      union {
        bool real;
        uint8_t base;
      } u_has_event;
      u_has_event.real = this->has_event;
      *(outbuffer + offset + 0) = (u_has_event.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->has_event);
      uint32_t length_events = strlen(this->events);
      varToArr(outbuffer + offset, length_events);
      offset += 4;
      memcpy(outbuffer + offset, this->events, length_events);
      offset += length_events;
      union {
        bool real;
        uint8_t base;
      } u_has_values;
      u_has_values.real = this->has_values;
      *(outbuffer + offset + 0) = (u_has_values.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->has_values);
      *(outbuffer + offset + 0) = (this->values_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->values_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->values_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->values_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->values_length);
      for( uint32_t i = 0; i < values_length; i++){
      union {
        double real;
        uint64_t base;
      } u_valuesi;
      u_valuesi.real = this->values[i];
      *(outbuffer + offset + 0) = (u_valuesi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_valuesi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_valuesi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_valuesi.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_valuesi.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_valuesi.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_valuesi.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_valuesi.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->values[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_unique_id;
      u_unique_id.base = 0;
      u_unique_id.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_unique_id.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_unique_id.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_unique_id.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->unique_id = u_unique_id.real;
      offset += sizeof(this->unique_id);
      union {
        int32_t real;
        uint32_t base;
      } u_frame_count;
      u_frame_count.base = 0;
      u_frame_count.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_frame_count.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_frame_count.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_frame_count.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->frame_count = u_frame_count.real;
      offset += sizeof(this->frame_count);
      union {
        double real;
        uint64_t base;
      } u_time;
      u_time.base = 0;
      u_time.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_time.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_time.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_time.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_time.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_time.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_time.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_time.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->time = u_time.real;
      offset += sizeof(this->time);
      uint32_t length_parent;
      arrToVar(length_parent, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_parent; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_parent-1]=0;
      this->parent = (char *)(inbuffer + offset-1);
      offset += length_parent;
      union {
        int32_t real;
        uint32_t base;
      } u_num_poses;
      u_num_poses.base = 0;
      u_num_poses.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_num_poses.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_num_poses.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_num_poses.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->num_poses = u_num_poses.real;
      offset += sizeof(this->num_poses);
      uint32_t poses_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      poses_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      poses_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      poses_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->poses_length);
      if(poses_lengthT > poses_length)
        this->poses = (geometry_msgs::Pose*)realloc(this->poses, poses_lengthT * sizeof(geometry_msgs::Pose));
      poses_length = poses_lengthT;
      for( uint32_t i = 0; i < poses_length; i++){
      offset += this->st_poses.deserialize(inbuffer + offset);
        memcpy( &(this->poses[i]), &(this->st_poses), sizeof(geometry_msgs::Pose));
      }
      union {
        bool real;
        uint8_t base;
      } u_has_event;
      u_has_event.base = 0;
      u_has_event.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->has_event = u_has_event.real;
      offset += sizeof(this->has_event);
      uint32_t length_events;
      arrToVar(length_events, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_events; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_events-1]=0;
      this->events = (char *)(inbuffer + offset-1);
      offset += length_events;
      union {
        bool real;
        uint8_t base;
      } u_has_values;
      u_has_values.base = 0;
      u_has_values.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->has_values = u_has_values.real;
      offset += sizeof(this->has_values);
      uint32_t values_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      values_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      values_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      values_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->values_length);
      if(values_lengthT > values_length)
        this->values = (double*)realloc(this->values, values_lengthT * sizeof(double));
      values_length = values_lengthT;
      for( uint32_t i = 0; i < values_length; i++){
      union {
        double real;
        uint64_t base;
      } u_st_values;
      u_st_values.base = 0;
      u_st_values.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_values.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_values.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_values.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_st_values.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_st_values.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_st_values.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_st_values.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->st_values = u_st_values.real;
      offset += sizeof(this->st_values);
        memcpy( &(this->values[i]), &(this->st_values), sizeof(double));
      }
     return offset;
    }

    const char * getType(){ return "osiris/GameObject"; };
    const char * getMD5(){ return "4c8b65052192fd26b30f4552ab839324"; };

  };

}
#endif
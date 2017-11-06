#include <iostream>
#include <string>
#include <stdio.h>
#include "ros.h"
#include <geometry_msgs\Point.h>
#include <geometry_msgs\Pose.h>
#include <std_msgs\String.h>
#include <std_msgs\Float64.h>
#include <geometry_msgs\Vector3.h>
#include <osiris\reaper_srv.h>
#include <osiris\ganesh_srv.h>
#include <osiris\comms_test_srv.h>
#include <osiris\Bone.h>
#include <osiris\GameObject.h>
#include <osiris\Values.h>
#include <Windows.h>
#include <vector>
#include <map>
#include <functional>

#define DllExport __declspec (dllexport)

// Prototypes
// TODO: Put in header
int startROSSerial(char *ip);
void DebugInUnity(std::string message);
void doubleCallback(double num);
void gOCallback(const osiris::GameObject & go);

extern "C" {
	DllExport struct unityPose {
		double px;
		double py;
		double pz;
		double ox;
		double oy;
		double oz;
		double ow;
	};
	DllExport struct values {
		char * name;
		double data;
	};
	DllExport struct gameObject {
		int unique_id;
		int frame_count;
		double time;
		const char * parent;
		unsigned int poses_length;
		unityPose * poses;
		bool has_event;
		unsigned int events_length;
		const char ** events;
		bool has_values;
		unsigned int values_length;
		const char ** values_name;
		double * values_data;
	};
	DllExport struct unityPoint {
		float px;
		float py;
		float pz;
	};
	DllExport struct accel {
		double x;
		double y;
		double z;
	};
}

// Extending ros publishers, subscribers with string topics
// TODO: Actually implement this.
class unityPublisher : public ros::Publisher {
public:
	std::string topic;
	unityPublisher(const char * topic_name, ros::Msg * msg) : ros::Publisher(topic_name, msg) {};
};

class unitySubscriber {
public:
	// WTF https://github.com/ros-drivers/rosserial/pull/241/commits/0a1ad9ac6585450424b9c80c6d5fdeb888e81386
	class subscriberData {
	public:
		bool lock;
		osiris::GameObject goData;
		void subscriberCallback(const osiris::GameObject & go)
		{
			if (!lock)
				goData = go;

			// TODO: Copy into object

			DebugInUnity(std::to_string(go.time));
		}
		subscriberData() { lock = false; };
		// ~subscriberData() {};
	};
	subscriberData subData;

	std::string topic;
	ros::Subscriber<osiris::GameObject, subscriberData> sub;

	unitySubscriber(const char * topic_name) : sub(topic_name, &subscriberData::subscriberCallback, &subData) {};

	osiris::GameObject fetch()
	{
		unlock();
		return osiris::GameObject(subData.goData);
	}

	// Is this really necessary?
	void lock()
	{
		subData.lock = true;
	}

	void unlock()
	{
		subData.lock = false;
	}
};

// Maps (Dictionaries)
std::map<int, ros::Publisher> publishers;
std::map<int, std::string> publisherTopics;
std::map<int, unitySubscriber> subscribers;
std::map<int, std::string> subscriberTopics;

// Messages (definitions)
geometry_msgs::Pose pose_msg;
geometry_msgs::Point point_msg;
geometry_msgs::Vector3 vector_msg;
osiris::Bone bone_msg;
osiris::GameObject go_msg;
std_msgs::String string_msg;
std_msgs::Float64 float_msg;

// Initialization
char rosmaster[256] = "localhost";
bool init = false;
ros::NodeHandle nh;
const char *reaper = "reaper";
const char *comms_test = "comms_test";
const char *ganesh = "ganesh";

// Services
ros::ServiceClient<osiris::reaper_srvRequest, osiris::reaper_srvResponse> reaper_srv(reaper);
ros::ServiceClient<osiris::comms_test_srvRequest, osiris::comms_test_srvResponse> comms_test_srv(comms_test);
ros::ServiceClient<osiris::ganesh_srvRequest, osiris::ganesh_srvResponse> ganesh_srv(ganesh);

enum messages { pose, point, str, acc, flt, bone };

extern "C" {
	// Unity Debug
	typedef void(__stdcall * DebugCallback) (const char *str);
	DebugCallback gDebugCallback;

	// Test Callback
	typedef void(__stdcall * TestCallback) (double number);
	TestCallback testCallback;

	DllExport void registerPublisher(int uid, char *_topic) {
		std::string topic = std::string(_topic);
		DebugInUnity("Registering " + topic + " with uid: " + std::to_string(uid));
		publisherTopics.insert(std::pair<int, std::string>(uid, topic));
		publishers.insert(std::pair<int, ros::Publisher>(uid, ros::Publisher(publisherTopics.at(uid).c_str(), &go_msg)));
	}

	DllExport void registerSubscriber(int uid, char *_topic) {
		std::string topic = std::string(_topic);
		DebugInUnity("Registering Subscriber " + topic + " with uid: " + std::to_string(uid));
		subscriberTopics.insert(std::pair<int, std::string>(uid, topic));
		subscribers.insert(std::pair<int, unitySubscriber>(uid, unitySubscriber(subscriberTopics.at(uid).c_str())));
	}

	DllExport int initNode(char *ip) {
		return startROSSerial(ip);
	}

	DllExport void advertise() {
		for (std::map<int, ros::Publisher>::iterator pb = publishers.begin(); pb != publishers.end(); ++pb)
		{
			DebugInUnity("Advertising " + std::string(pb->second.topic_)  + " with uid: " + std::to_string(pb->first));
			nh.advertise(pb->second);
		}

		for (std::map<int, unitySubscriber>::iterator sb = subscribers.begin(); sb != subscribers.end(); ++sb)
		{
			DebugInUnity("Subscribing to " + std::string(sb->second.sub.topic_) + " with uid: " + std::to_string(sb->first));
			nh.subscribe(sb->second.sub);
		}

		nh.serviceClient(reaper_srv);
		nh.serviceClient(comms_test_srv);
		nh.serviceClient(ganesh_srv);
	}

	DllExport void advertiseSingle(int uid) {
		// If find returns the end of the map (past last element)
		std::map<int, ros::Publisher>::iterator pb = publishers.find(uid);
		if (pb != publishers.end())
		{
			DebugInUnity("Advertising " + std::string(pb->second.topic_) + " with uid: " + std::to_string(pb->first));
			nh.advertise(pb->second);
		}
		nh.negotiateTopics();
	}

	DllExport void spinOnce() {
		nh.spinOnce();
	}

	DllExport void closeNode() {
		publishers.clear();
		subscribers.clear();
		publisherTopics.clear();
		subscriberTopics.clear();
	}

	DllExport void publish(int uid, gameObject _go) {
		osiris::GameObject go;
		go.unique_id = uid;
		go.frame_count = _go.frame_count;
		go.time = _go.time;
		go.parent = _go.parent;
		go.poses_length = _go.poses_length;
		go.num_poses = _go.poses_length;
		go.poses = new geometry_msgs::Pose[go.poses_length];
		for (int i = 0; i != go.poses_length; i++)
		{
			go.poses[i].position.x = _go.poses[i].px;
			go.poses[i].position.y = _go.poses[i].py;
			go.poses[i].position.z = _go.poses[i].pz;
			go.poses[i].orientation.x = _go.poses[i].ox;
			go.poses[i].orientation.y = _go.poses[i].oy;
			go.poses[i].orientation.z = _go.poses[i].oz;
			go.poses[i].orientation.w = _go.poses[i].ow;
		}
		
		go.has_event = _go.has_event;
		if (go.has_event)
		{
			go.events_length = _go.events_length;
			go.events = new char*[go.events_length];
			for (int i = 0; i != go.events_length; i++)
			{
				go.events[i] = (char *)_go.events[i];
			}
		}

		go.has_values = _go.has_values;
		
		if (go.has_values) 
		{
			go.values_length = _go.values_length;
			go.values = new osiris::Values[go.values_length];
			for (int i = 0; i != go.values_length; i++)
			{
				go.values[i].name = _go.values_name[i];
				go.values[i].data = _go.values_data[i];
			}
		}

		publishers.at(uid).publish(&go);
	}

	DllExport double fetch(int uid)
	{
		subscribers.at(uid).lock();
		DebugInUnity(std::to_string(subscribers.at(uid).fetch().time));
		return subscribers.at(uid).fetch().time;
	}

	DllExport void unityShutdown() {
		osiris::reaper_srvRequest req;
		osiris::reaper_srvResponse res;
		req.input = "unityshutdown";
		reaper_srv.call(req, res);
	}

	DllExport void shimmerShutdown() {
		osiris::reaper_srvRequest req;
		osiris::reaper_srvResponse res;
		req.input = "shimmershutdown";
		reaper_srv.call(req, res);
	}

	DllExport void commsTest(char* _text, char* ret) {
		std::string text = _text;
		osiris::comms_test_srvRequest req;
		osiris::comms_test_srvResponse res;
		req.input = text.c_str();
		comms_test_srv.call(req, res);
		strncpy(ret, res.hash, 32);
	}

	DllExport int beginRecord(char* _filename, char* _topics, char* ret) {
		std::string filename = _filename;
		std::string topics = _topics;
		osiris::ganesh_srvRequest req;
		osiris::ganesh_srvResponse res;
		req.command = "recordbegin";
		req.filename = filename.c_str();

		/* For another day
		std::vector<std::string> topicsSplit;
		std::string delim = " ";

		int numTopics = 0;
		auto start = 0U;
		auto end = topics.find(delim);
		while (end != std::string::npos) {
		topicsSplit.push_back(topics.substr(start, end - start));
		start = end + delim.length();
		end = topics.find(delim, start);
		numTopics++;
		}
		req.topics_length = numTopics;

		char** topicsOut = (char**)calloc((size_t)numTopics, sizeof(char*));

		req.topics = topicsOut;
		for (int i = 0; i < numTopics; i++) {

		}*/

		req.topics = topics.c_str();
		ganesh_srv.call(req, res);
		strncpy(ret, res.bag_output, 256);
		return res.success;
	}

	DllExport void endRecord() {
		osiris::ganesh_srvRequest req;
		osiris::ganesh_srvResponse res;
		req.command = "recordend";
		req.filename = "";
		req.topics = "";
		ganesh_srv.call(req, res);
	}

	// TODO? Maybe implement a way to get specify the bag we want scrapped?
	// instead of sorting by newest?
	DllExport void scrapRecord() {
		osiris::ganesh_srvRequest req;
		osiris::ganesh_srvResponse res;
		req.command = "recordend_scrap";
		req.filename = "";
		req.topics = "";
		ganesh_srv.call(req, res);
	}

	DllExport void RegisterDebugCallback(DebugCallback callback)
	{
		if (callback)
		{
			gDebugCallback = callback;
		}
	}
}

int startROSSerial(char *_ip) {
	// ROS Master @ Ubuntu on VM
	strcpy(rosmaster, _ip);
	// TODO: Add try-catch
	int result = 1;
	result = nh.initNode(rosmaster);
	if (result)
		DebugInUnity("Connected to rosserial-server");
	else
		DebugInUnity("Failed to connect to resserial-server");

	// ros::Subscriber<osiris::GameObject>("testlisten",  &gOCallback);

	init = true;
	return result;
}

void DebugInUnity(std::string message)
{
	if (gDebugCallback)
	{
		gDebugCallback(message.c_str());
	}
}

void doubleCallback(double num)
{
	if (testCallback)
	{
		testCallback(num);
	}
}


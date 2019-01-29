// ROS Csereal
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System;
using System.Text;
using System.Text.RegularExpressions;

using UnityEngine;

/// <summary>
/// ROS csereal. "Mother" of all ROS publishers and subscribers in current scene.
/// </summary>

public class ROSCsereal : MonoBehaviour
{
    #region Native Interop
    // ROS-Csereal Pointer

    static IntPtr rcptr;

    [DllImport("ros-csereal-unity")]
    static extern int initNode(string ip);

    [DllImport("ros-csereal-unity")]
    static extern void registerPublisher(int uid, string topic);

    [DllImport("ros-csereal-unity")]
    static extern void registerSubscriber(int uid, string topic);

    [DllImport("ros-csereal-unity")]
    static extern void publish(int uid, rosGameObject rGO);

    [DllImport("ros-csereal-unity")]
    static extern void advertise();

    [DllImport("ros-csereal-unity")]
    static extern void advertiseSingle(int uid);

    [DllImport("ros-csereal-unity")]
    static extern IntPtr fetch(int uid);

    [DllImport("ros-csereal-unity")]
    static extern void spinOnce();

    [DllImport("ros-csereal-unity")]
    static extern void unityShutdown();

    [DllImport("ros-csereal-unity")]
    static extern void closeNode();

    private delegate void DebugCallback(string message);

    [DllImport("ros-csereal-unity")]
    static extern void RegisterDebugCallback(DebugCallback callback);

    [StructLayout(LayoutKind.Sequential)]
    public class rosGameObject
    {
        public int unique_id;
        public int frame_count;
        public double time;
        public IntPtr parent;
        public uint poses_length;
        public IntPtr poses;
        public bool has_events;
        public uint events_length;
        public IntPtr events;
        public bool has_values;
        public uint values_length;
        public IntPtr values_name;
        public IntPtr values_data;
    }

    [StructLayout(LayoutKind.Sequential)]
    public class unityPose
    {
        public double px;
        public double py;
        public double pz;
        public double ox;
        public double oy;
        public double oz;
        public double ow;
    }
    #endregion

    public List<ROSPublisher> publishers;

    public List<ROSPublisher> physicsPublishers; // TODO

	public List<ROSSubscriber> subscribers;

	public List<ROSSubscriber> physicsSubscribers;

    private Dictionary<int, ROSPublisher> registeredPublishers;

	private Dictionary<int, ROSSubscriber> registeredSubscribers;

	private Dictionary<int, ROSSubscriber> registeredPhysicsSubscribers;

    private bool serialConnected = false;

    public string rosIP;

    // Script Execution is important! ROSCsereal must be after all ROSPublishers ROSSubscribers!
    void Start()
    {
        // Load ros-csereal library
        rcptr = Native.LoadLibrary("ros-csereal-unity");

        RegisterDebugCallback(new DebugCallback(DebugMethod));

        registeredPublishers = new Dictionary<int, ROSPublisher>();
		registeredSubscribers = new Dictionary<int, ROSSubscriber> ();
		registeredPhysicsSubscribers = new Dictionary<int, ROSSubscriber> ();

		// TODO: Put in method
        foreach (var rp in publishers)
        {
            if (rp == null)
            {
                logCserealError("Publisher empty");
                continue;
            }
            
            int uid = rp.GetInstanceID();
            if (!registeredPublishers.ContainsKey(uid))
            {
                registeredPublishers.Add(uid, rp.register());
                registerPublisher( uid, rp.topic);
            }
            else
            {
                logCserealError("Object with ID: " + uid + " already exists.");
            }
        }

		foreach (var sub in subscribers)
		{
			if (sub == null)
            {
                logCserealError("Subscriber empty");
                continue;
            }

			int uid = sub.GetInstanceID();
			if (!registeredPhysicsSubscribers.ContainsKey(uid))
			{
				registeredSubscribers.Add(uid, sub.register());
				registerSubscriber( uid, sub.topic);
			}
			else
			{
				logCserealError("Object with ID: " + uid + " already exists.");
			}
		}

		foreach (var sub in physicsSubscribers)
		{
			if (sub == null)
            {
                logCserealError("Physics Subscriber empty");
                continue;
            }

			int uid = sub.GetInstanceID();
			if (!registeredPhysicsSubscribers.ContainsKey(uid))
			{
				registeredPhysicsSubscribers.Add(uid, sub.register());
				registerSubscriber( uid, sub.topic);
			}
			else
			{
				logCserealError("Object with ID: " + uid + " already exists.");
			}
		}

        if (rosIP == "")
        {
            Debug.LogError("[ROS] Specify IP!");
        }

        int rosConnection = initNode(rosIP);
        if (rosConnection != 1)
        {
            Debug.LogError("[ROS] ROSSerial Connection Failed");
            return;
        } else {
            serialConnected = true;
        }

        System.Threading.Thread.Sleep(50);

        advertise();
        spinOnce();

    }

	void FixedUpdate()
	{
        if (!serialConnected)
        {
            return;
        }
        spinOnce();

		foreach (var sub in registeredPhysicsSubscribers)
		{
            if (sub.Value == null)
                continue;
            
            rosGameObject _recv = new rosGameObject();

            sub.Value.clearEvents();

            try
            {
                Marshal.PtrToStructure(fetch(sub.Key), _recv);
            }
            catch (Exception ex)
            {
				continue;
            }

            sub.Value.parent = Marshal.PtrToStringAnsi(_recv.parent);

            IntPtr[] _poses = new IntPtr [_recv.poses_length];
            if (_recv.poses_length > 0)
            {
                Marshal.Copy(_recv.poses, _poses, 0, (int)_recv.poses_length);
            }

            for (int i = 0; i < _recv.poses_length; ++i){
                // Only update what we can
                if (i > sub.Value.poses.Count - 1)
                    continue;

                unityPose _pose = new unityPose();
                Marshal.PtrToStructure(_poses[i], _pose);

                 sub.Value.poses[i].position = new Vector3((float)_pose.px, (float)_pose.py, (float)_pose.pz);
                 sub.Value.poses[i].rotation = new Quaternion((float)_pose.ox, (float)_pose.oy, (float)_pose.oz, (float)_pose.ow);
            }


            IntPtr[] _events_ptr = new IntPtr[_recv.events_length];
            if (_recv.events_length > 0)
            {
                Marshal.Copy(_recv.events, _events_ptr, 0, (int)_recv.events_length);
            }
            for (int i = 0; i < _recv.events_length; ++i)
            {
                sub.Value.logEvent(Marshal.PtrToStringAnsi(_events_ptr[i]));
            }

            double[] _values_data = new double[_recv.values_length];
            IntPtr[] _values_name_ptr = new IntPtr[_recv.values_length];
            if (_recv.values_length > 0)
            {
                Marshal.Copy(_recv.values_name, _values_name_ptr, 0, (int)_recv.values_length);
                Marshal.Copy(_recv.values_data, _values_data, 0, (int)_recv.values_length);
            }
            for (int i = 0; i < _recv.values_length; ++i)
            {
                string name = Marshal.PtrToStringAnsi(_values_name_ptr[i]);
                if(!sub.Value.values.setValue(name, _values_data[i]))
                {
                    ROSSubscriber.logSubscriberError("Could not find value named: " + name);
                }
            }
		}
	}

    void Update()
    {
        if (!serialConnected)
        {
            return;
        }
        spinOnce();

        foreach (var sub in registeredSubscribers)
        {
            if (sub.Value == null)
                continue;
            
            rosGameObject _recv = new rosGameObject();

            sub.Value.clearEvents();

            try
            {
                Marshal.PtrToStructure(fetch(sub.Key), _recv);
            }
            catch (Exception ex)
            {
                continue;
            }

            sub.Value.parent = Marshal.PtrToStringAnsi(_recv.parent);

            IntPtr[] _poses = new IntPtr [_recv.poses_length];
            if (_recv.poses_length > 0)
            {
                Marshal.Copy(_recv.poses, _poses, 0, (int)_recv.poses_length);
            }

            for (int i = 0; i < _recv.poses_length; ++i){
                // Only update what we can
                if (i > sub.Value.poses.Count - 1)
                    continue;

                unityPose _pose = new unityPose();
                Marshal.PtrToStructure(_poses[i], _pose);

                sub.Value.poses[i].position = new Vector3((float)_pose.px, (float)_pose.py, (float)_pose.pz);
                sub.Value.poses[i].rotation = new Quaternion((float)_pose.ox, (float)_pose.oy, (float)_pose.oz, (float)_pose.ow);
            }

            IntPtr[] _events_ptr = new IntPtr[_recv.events_length];
            if (_recv.events_length > 0)
            {
                Marshal.Copy(_recv.events, _events_ptr, 0, (int)_recv.events_length);
            }
            for (int i = 0; i < _recv.events_length; ++i)
            {
                sub.Value.logEvent(Marshal.PtrToStringAnsi(_events_ptr[i]));
            }

            double[] _values_data = new double[_recv.values_length];
            IntPtr[] _values_name_ptr = new IntPtr[_recv.values_length];
            if (_recv.values_length > 0)
            {
                Marshal.Copy(_recv.values_name, _values_name_ptr, 0, (int)_recv.values_length);
                Marshal.Copy(_recv.values_data, _values_data, 0, (int)_recv.values_length);
            }
            for (int i = 0; i < _recv.values_length; ++i)
            {
                string name = Marshal.PtrToStringAnsi(_values_name_ptr[i]);
                if(!sub.Value.values.setValue(name, _values_data[i]))
                {
                    ROSSubscriber.logSubscriberError("Could not find value named: " + name);
                }
            }
        }
    }

    void LateUpdate()
    {
        int frame_count = Time.frameCount;
        double time = Time.time;

        #region ROSCsereal (Must be last!)
        if (!serialConnected)
        {
            return;
        }

        // Spin for subscribers, should be blocking
        spinOnce();

        // Publish topics
        foreach (var rp in registeredPublishers)
        {
            if (rp.Value == null)
                continue;
            
            rosGameObject rGO = new rosGameObject();

            // Headers
            rGO.unique_id = rp.Key;
            rGO.frame_count = frame_count;
            rGO.time = time;

            // Parent (Immutable)
			rGO.parent = Marshal.StringToCoTaskMemAnsi(rp.Value.parent);

            // Poses (Transforms)
            rGO.poses_length = (uint)rp.Value.poses.Count;

            IntPtr posesPtr = Marshal.AllocCoTaskMem(Marshal.SizeOf(new unityPose()) * (int)rGO.poses_length);
            if (rGO.poses_length > 0) 
            {
                double[] temp = new double[7 * rGO.poses_length];
                for (int i = 0; i != rGO.poses_length; ++i) {
                    temp[7*i] = rp.Value.poses[i].position.x;
                    temp[7*i + 1] = rp.Value.poses[i].position.y;
                    temp[7*i + 2] = rp.Value.poses[i].position.z;
                    temp[7*i + 3] = rp.Value.poses[i].rotation.x;
                    temp[7*i + 4] = rp.Value.poses[i].rotation.y;
                    temp[7*i + 5] = rp.Value.poses[i].rotation.z;
                    temp[7*i + 6] = rp.Value.poses[i].rotation.w;
                }
                Marshal.Copy(temp, 0, posesPtr, 7 * (int)rGO.poses_length);
            }
            rGO.poses = posesPtr;

            // Events
            rGO.events_length = (uint)rp.Value.events.Count;
            rGO.has_events = rGO.events_length > 0 ? true : false; // Shorthand because I'm cool
            IntPtr[] tempEventsPtrArray = new IntPtr [rGO.events_length];

            if (rGO.has_events && rGO.events_length > 0)
            {
                rGO.events = Marshal.AllocCoTaskMem(Marshal.SizeOf(new IntPtr()) * rp.Value.events.Count);

                int iterEvents = 0;
                foreach (var _event in rp.Value.events)
                {
                    tempEventsPtrArray[iterEvents] = Marshal.StringToCoTaskMemAnsi(_event);
                    iterEvents++;
                }
                Marshal.Copy(tempEventsPtrArray, 0, rGO.events, (int)rGO.events_length);
            }

            // Values (Attributes or whatever)
            rGO.values_length = (uint)rp.Value.values.val.Count;
            rGO.has_values = rGO.values_length > 0 ? true : false; // Shorthand because I'm cool

            // NOTE: Do not name variables "value".
            rGO.values_name = Marshal.AllocCoTaskMem(Marshal.SizeOf(new IntPtr()) * (int)rGO.values_length);
            IntPtr[] tempValuesNameArray = new IntPtr [rGO.values_length];
            rGO.values_data =  Marshal.AllocCoTaskMem(Marshal.SizeOf(new double()) * (int)rGO.values_length);
            double[] tempValuesDataArray = new double [rGO.values_length];
            int iterValues = 0;
            foreach (var val in rp.Value.values.val)
            {
                tempValuesNameArray[iterValues] = Marshal.StringToCoTaskMemAnsi(val.Key);
                tempValuesDataArray[iterValues] = val.Value;
                iterValues++;
            }
            Marshal.Copy(tempValuesNameArray, 0, rGO.values_name, (int)rGO.values_length);
            Marshal.Copy(tempValuesDataArray, 0, rGO.values_data, (int)rGO.values_length);

            // Publishing
            publish( rp.Key, rGO);

            // Housekeeping
            rp.Value.clearEvents();
            Marshal.FreeCoTaskMem(posesPtr);
            foreach (var item in tempEventsPtrArray)
            {
                Marshal.FreeCoTaskMem(item);
            }
            foreach (var item in tempValuesNameArray)
            {
                Marshal.FreeCoTaskMem(item);
            }
            Marshal.FreeCoTaskMem(rGO.values_data);
            Marshal.FreeCoTaskMem(rGO.values_name);
        }

        spinOnce();
        #endregion
    }

    void OnDisable()
    {
        // Potential memory leak?
        // closeNode();

        if (serialConnected)
        {
            unityShutdown();
        }

        Debug.Log(Native.FreeLibrary(rcptr)
            ? "ROSSerial DLL Unloaded"
            : "ROSSerial DLL Failed to Unload");
        rcptr = IntPtr.Zero;
    }

	/// <summary>
	/// This is for creating publishers after the initial connection
	/// </summary>
	public void lateRegister(ROSPublisher latePublisher)
	{
		int uid = latePublisher.GetInstanceID ();
		registeredPublishers.Add(uid, latePublisher.register());
		registerPublisher( uid, latePublisher.topic);

		advertiseSingle(uid);
		spinOnce();
	}

    public static void logCserealError(string message)
    {
        Debug.Log("<color=#7500AF>[ROSCsereal]: " + message + "</color>");
    }

    private static void DebugMethod(string message)
    {
        Debug.Log("<color=#FF3700>[ROSCsereal-Native]: " + message + "</color>");
    }

}

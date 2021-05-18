﻿using OpenNETCF.MQTT;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.ServiceProcess;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace PcClientService
{
    public partial class PcClientService : ServiceBase
    {

        [DllImport("Kernel32")]
        private static extern bool SetConsoleCtrlHandler(EventHandler handler, bool add);

        private delegate bool EventHandler(CtrlType sig);
        static EventHandler _handler;

        enum CtrlType
        {
            CTRL_C_EVENT = 0,
            CTRL_BREAK_EVENT = 1,
            CTRL_CLOSE_EVENT = 2,
            CTRL_LOGOFF_EVENT = 5,
            CTRL_SHUTDOWN_EVENT = 6
        }
        private static bool Handler(CtrlType sig)
        {

            try
            {
                client.Publish(System.Environment.MachineName + "/out/SHUTDOWN", "now", QoS.FireAndForget, true);
            }
            catch (Exception)
            { }
            Thread.Sleep(5000);
            Environment.Exit(-1);
            return true;
        }
        public PcClientService()
        {
            InitializeComponent();
        }
        private static MQTTClient client = new MQTTClient("192.168.2.1", 1883);
        System.Timers.Timer timer = new System.Timers.Timer();
   //     private AudioSwitcher.AudioApi.CoreAudio.CoreAudioDevice AudioDevice;
        //private AudioSwitcher.AudioApi.CoreAudio.CoreAudioController AudioControl;
        protected override void OnStart(string[] args)
        {
            _handler += new EventHandler(Handler);
            SetConsoleCtrlHandler(_handler, true);
            OnTimer(null, null);
    //        AudioDevice = new AudioSwitcher.AudioApi.CoreAudio.CoreAudioController().DefaultPlaybackDevice;
    //        AudioControl.DefaultPlaybackDevice
            timer.Interval = 1000; // 60 seconds
            timer.Enabled = true;
            timer.Elapsed += new System.Timers.ElapsedEventHandler(this.OnTimer);
            timer.Start();
        }


        void OnTimer(object sender, EventArgs e)
        {                // wait for the connection to complete
            if (!client.IsConnected)
            {
                try
                {
                    client = new MQTTClient("mqtt.burnsys.net.ar", 1883);

                    // hook up the MessageReceived event with a handler
                    client.MessageReceived += (topic, qos, payload) =>
                    {
                        var command = topic.Split('/')[2].ToUpper().Trim();
                        var payloadst = Encoding.UTF8.GetString(payload, 0, payload.Length);
                        switch (command)
                        {
                            case "RUN":
                                try
                                {
                                    System.Diagnostics.Process.Start(payloadst);
                                }
                                catch (Exception)
                                {
                                }
                                break;
                            case "SHUTDOWN":
                                var psi = new ProcessStartInfo("shutdown", "/p /f");
                                psi.CreateNoWindow = true;
                                psi.UseShellExecute = false;
                                Process.Start(psi);
                                break;
                            //case "VOLUME":
                            //    AudioDevice.Volume = int.Parse(payloadst);
                            //    break;
                            //case "MUTE":
                            //    AudioDevice.ToggleMute();
                            //    break;
                        }
                    };
                    client.Connect(System.Environment.MachineName);
                    client.Subscriptions.Add(new Subscription(System.Environment.MachineName + "/in/#"));
                    client.Publish(System.Environment.MachineName + "/out/IP", GetLocalIPAddress(), QoS.FireAndForget, false);

                }
                catch (Exception)
                {
                    return;
                }
            }

            // publish on our own subscribed topic to see if we hear what we send
            client.Publish(System.Environment.MachineName + "/out/lastseen/epoch", DateTime.Now.ToEpochMilliseconds().ToString(), QoS.FireAndForget, false);
            //if (AudioDevice != null)
            //{
            //    client.Publish(System.Environment.MachineName + "/out/volume", AudioDevice.Volume.ToString(), QoS.FireAndForget, false);
            //    client.Publish(System.Environment.MachineName + "/out/Mute", (AudioDevice.IsMuted ? 1:0).ToString(), QoS.FireAndForget, false);
            //}
        }

        protected override void OnStop()
        {
            try
            {
                client.Publish(System.Environment.MachineName + "/out/SHUTDOWN", "now", QoS.FireAndForget, true);
            }
            catch (Exception)
            { }
            Thread.Sleep(2000);

        }
        public static string GetLocalIPAddress()
        {
            var host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (var ip in host.AddressList)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    return ip.ToString();
                }
            }
            return string.Empty;
            // throw new Exception("No network adapters with an IPv4 address in the system!");
        }
    }
}

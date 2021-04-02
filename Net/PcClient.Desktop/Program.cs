using OpenNETCF.MQTT;
using System;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace PcClient.Desktop
{
    class Program
    {
        [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
        static extern bool FreeConsole();

        [System.Runtime.InteropServices.DllImport("User32.dll")]
        private static extern bool SetForegroundWindow(IntPtr handle);
        private static IntPtr handle;

        private static MQTTClient client = new MQTTClient("192.168.2.1", 1883);
        static System.Timers.Timer timer = new System.Timers.Timer();

        static void Main(string[] args)
        {
            FreeConsole();
            OnTimer(null, null);
            timer.Interval = 1000; // 60 seconds
            timer.Enabled = true;
            timer.Elapsed += new System.Timers.ElapsedEventHandler(OnTimer);
            timer.Start();
            while (true)
            {
                Thread.Sleep(2000);
            }
        }

        static void OnTimer(object sender, EventArgs e)
        {                // wait for the connection to complete
            if (!client.IsConnected)
            {
                try
                {
                    client = new MQTTClient("192.168.2.1", 1883);

                    // hook up the MessageReceived event with a handler
                    client.MessageReceived += (topic, qos, payload) =>
                    {
                        var command = topic.Split('/')[3].ToUpper().Trim();
                        var payloadst = Encoding.UTF8.GetString(payload, 0, payload.Length);
                        switch (command)
                        {
                            case "RUN":
                                try
                                {
                                    var args = payloadst.Split(',');
                                    System.Diagnostics.Process proc = new System.Diagnostics.Process();
                                    proc.StartInfo.FileName = args[0];
                                    if (args.Length == 2)
                                        proc.StartInfo.Arguments = args[1];
                                    proc.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Maximized; // it Maximized application  
                                    proc.Start();
                              //      Thread.Sleep(2000);
                                    SetForegroundWindow(proc.MainWindowHandle);
                                }

                                catch (Exception)
                                {}
                                break;
                        }
                    };
                    client.Connect(System.Environment.MachineName + "/desktop");
                    client.Subscriptions.Add(new Subscription(System.Environment.MachineName + "/desktop/in/#"));
                    client.Publish(System.Environment.MachineName + "/desktop/out/IP", GetLocalIPAddress(), QoS.FireAndForget, false);

                }
                catch (Exception)
                {
                    return;
                }
            }

            // publish on our own subscribed topic to see if we hear what we send
            client.Publish(System.Environment.MachineName + "/desktop/out/lastseen/epoch", DateTime.Now.ToEpochMilliseconds().ToString(), QoS.FireAndForget, false);
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

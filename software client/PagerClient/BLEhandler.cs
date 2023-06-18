using InTheHand.Net.Bluetooth;
using InTheHand.Net.Sockets;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Toolkit.Uwp.Notifications;
using System.ComponentModel;
using Microsoft.Windows.AppNotifications.Builder;

namespace PagerClient
{



    public class BluetoothDevice
    {
        public string Name { get; set; }
        public string Address { get; set; }
    }

    public class BLEhandler
    {
        static string startbluetoothloopbackMAKRO = "~";
        static NetworkStream nstream;

        static private string buffermsg;
           
        static private bool notificationsfromlora = false;


        

        public static List<string> nearbyBluetootDevices()
        {
            List<string> devices = new List<string>();
            BluetoothClient client = new BluetoothClient();
            IReadOnlyCollection<BluetoothDeviceInfo> devicesdiscovered = client.DiscoverDevices();
            foreach (BluetoothDeviceInfo d in devicesdiscovered)
            {
                devices.Add(d.DeviceName);
            }
            return devices;
        }



        public static NetworkStream connectBLE(string deviceName)
        {
            BluetoothClient client = new BluetoothClient();
            IReadOnlyCollection<BluetoothDeviceInfo> devicesdiscovered = client.DiscoverDevices();
            foreach (BluetoothDeviceInfo d in devicesdiscovered)
            {
                if (d.DeviceName == deviceName)
                {
                    client.Connect(d.DeviceAddress, BluetoothService.SerialPort);
                    NetworkStream stream = client.GetStream();

                    stream.Write(Encoding.ASCII.GetBytes(startbluetoothloopbackMAKRO), 0, Encoding.ASCII.GetBytes(startbluetoothloopbackMAKRO).Length);
                    stream.Flush();
                    Debug.WriteLine("Connected to " + d.DeviceName);
                    Debug.WriteLine("Address: " + d.DeviceAddress);
                    Debug.WriteLine("BluetoothLoopback started");
                    notificationsfromlora = true;
                    return stream;
                }
            }
            return null;
        }

        public static void selectionListViewBle(ListView list, RoutedEventArgs e)
        {
            string selecteditem = list.SelectedItem.ToString();
            nstream = connectBLE(selecteditem);

            while (true)
            {

                ReadNetworkStreamWithAscii(nstream);
            }
            //notificationBLE();
        }


        public static void sendBLE(NetworkStream stream, string msg)
        {
            byte[] msgbytes = Encoding.ASCII.GetBytes(msg);
            stream.Write(msgbytes, 0, msgbytes.Length);
            stream.Flush();
        }

        public static string reciveBLE(NetworkStream stream)
        {
            byte[] msgbytes = new byte[1024];
            stream.Read(msgbytes, 0, msgbytes.Length);
            string msg = Encoding.ASCII.GetString(msgbytes);
            return msg;
        }

        public static void notificationBLE()
        {
            while (true)
            {
                if (notificationsfromlora == true)
                {
                    byte[] msgbytes = new byte[1024];
                    nstream.Read(msgbytes, 0, msgbytes.Length);
                    string Msg = Encoding.ASCII.GetString(msgbytes);
                    Debug.WriteLine("");
                    Debug.WriteLine("INCOMMING: "+ Msg);
                    Debug.WriteLine("");

                    

                    /*

                    new ToastContentBuilder()
                        .AddArgument("action", "viewConversation")
                        .AddText("New Message")
                        .AddText(Msg)
                        .Show();
                        */
                    
                }
            }
   
        }

        public static void ReadNetworkStreamWithAscii(NetworkStream stream)
        {
            byte[] buffer = new byte[1024];
            StringBuilder asciiData = new StringBuilder();

            while (stream.DataAvailable)
            {
                int bytesRead = stream.Read(buffer, 0, buffer.Length);
                string receivedData = Encoding.ASCII.GetString(buffer, 0, bytesRead);
                asciiData.Append(receivedData);
            }

            string fullData = asciiData.ToString();
            Debug.WriteLine(fullData);
            if (IsAsciiOnly(fullData))
            {
                new ToastContentBuilder()
                       .AddArgument("action", "viewConversation")
                       .AddText("New Message")
                       .AddText(fullData)
                       .Show();
            }
            else
            {
                Console.WriteLine("Received data contains non-ASCII characters.");
            }
        }

        private static bool IsAsciiOnly(string data)
        {
            foreach (char c in data)
            {
                if ((int)c > 127)
                    return false;
                if((int)c < 0) return false;

            }

            return true;
        }
    }
}

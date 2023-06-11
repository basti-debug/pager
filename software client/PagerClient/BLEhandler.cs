using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InTheHand.Net;
using InTheHand.Net.Bluetooth;
using InTheHand.Net.Sockets;

namespace PagerClient
{
    public class BLEhandler
    {
        public static List<string> nearbyBLE()
        {
            List<string> nearby = new List<string>();
            BluetoothClient client = new BluetoothClient();
            BluetoothDeviceInfo[] devices = client.DiscoverDevices();
            foreach (BluetoothDeviceInfo d in devices)
            {
                nearby.Add(d.DeviceName);
            }
            return nearby;
        }

        public void sendoverBLE(string msg)
        {
              BluetoothClient client = new BluetoothClient();
            BluetoothDeviceInfo[] devices = (BluetoothDeviceInfo[])client.DiscoverDevices();
            foreach (BluetoothDeviceInfo d in devices)
            {
                if (d.DeviceName == "Pager")
                {
                    BluetoothAddress address = d.DeviceAddress;
                    BluetoothEndPoint ep = new BluetoothEndPoint(address, BluetoothService.SerialPort);
                    BluetoothClient bc = new BluetoothClient();
                    bc.Connect(ep);
                    System.IO.Stream peerStream = bc.GetStream();
                    byte[] buffer = Encoding.UTF8.GetBytes(msg);
                    peerStream.Write(buffer, 0, buffer.Length);
                    peerStream.Close();
                    bc.Close();
                }
            }
        }

        /*
        public string HandleBLErecive(int port)
        {

        }

        public void sendBLE(int port, string msg) 
        { 
        
        } */
    }
}

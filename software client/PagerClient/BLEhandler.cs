using InTheHand.Net.Sockets;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PagerClient
{
    public class BLEhandler
    {
        public static List<string> nearbyBluetooth()
        {
            BluetoothClient client = new BluetoothClient();
            List<string> items = new List<string>();
           /* BluetoothDeviceInfo[] devices = client.DiscoverDevicesInRange();
            foreach (BluetoothDeviceInfo d in devices)
            {
                items.Add(d.DeviceName);
            }*/
            return items;
        }
        /*
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
        }+/

        /*
        public string HandleBLErecive(int port)
        {

        }

        public void sendBLE(int port, string msg) 
        { 
        
        } */
    }
}

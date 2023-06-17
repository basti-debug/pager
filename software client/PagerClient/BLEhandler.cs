using InTheHand.Net.Sockets;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PagerClient
{

    public class BluetoothDevice
    {
        public string Name { get; set; }
        public string Address { get; set; }
    }

    public class BLEhandler
    {

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


        
         

        /*
        public string HandleBLErecive(int port)
        {

        }

        public void sendBLE(int port, string msg) 
        { 
        
        } */
    }
}

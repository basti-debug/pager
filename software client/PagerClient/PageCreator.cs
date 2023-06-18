using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Shapes;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Media.Capture;
using Windows.Storage;
using Windows.UI;
using InTheHand.Net.Sockets;
using System.Collections.ObjectModel;
using Windows.Devices.Enumeration;
using System.Security.Cryptography.X509Certificates;

namespace PagerClient
{
    public class PageCreator
    {
        #region variables

        // standard grid 
        Grid aGrid = new Grid();


        ListView deviceslist = new ListView();

        // Theme

        public static string theme = "Default";
        private SolidColorBrush accent = new SolidColorBrush((Windows.UI.Color)Application.Current.Resources["SystemAccentColor"]);

        #endregion


        public void displayChangeSettings(Canvas canva)
        {

        }

        public void displayMainPage(Canvas currentframe)
        {
            currentframe.Children.Clear();

            TextBlock Title = new TextBlock();
            Title.Text = "Pager Client";
            Title.FontSize = 30;

            TextBlock info1 = new TextBlock();
            info1.Text = "Version 1";
            info1.FontSize = 15;

            TextBlock lstmsg = new TextBlock();
            lstmsg.Text = "Messages: ";
            lstmsg.FontSize = 15;



            ListView msglist = new ListView();
            
            
            msglist.Width = 500;

            Canvas.SetLeft(msglist, 50);
            Canvas.SetTop(msglist, 150);

            currentframe.Children.Add(msglist);

            if(msglist.Items.Count == 0)
            {
                TextBlock noMessages = new TextBlock();
                noMessages.Text = "No messages found, you may need to firstly connect to a pager";
                noMessages.FontSize = 15;

                Canvas.SetLeft(noMessages, 50);
                Canvas.SetTop(noMessages, 180);
                currentframe.Children.Add(noMessages);
            }

            // Moving Objects inside the Canvas
            Canvas.SetLeft(Title, 50);
            Canvas.SetTop(Title, 50);

            Canvas.SetLeft(info1, 50);
            Canvas.SetTop(info1, 100);

            Canvas.SetLeft(lstmsg, 50);
            Canvas.SetTop(lstmsg, 125);

            // Adding Objects to the Canvas

            
            currentframe.Children.Add(Title);
            currentframe.Children.Add(info1);
            currentframe.Children.Add(lstmsg);
        }

        public void displaySettingsPage(Canvas canva)
        {
            canva.Children.Clear();
            TextBlock Title = new TextBlock();
            Title.Text = "Settings";
            Title.FontSize = 30;

            Canvas.SetLeft(Title, 50);
            Canvas.SetTop(Title, 100);

            canva.Children.Add(Title);
        }

        public void connectbuttonclick(object sender, RoutedEventArgs e) 
        {
            BLEhandler.selectionListViewBle(deviceslist, e);
        }


        public void displayConnectionPage(Canvas canva)
        {
            canva.Children.Clear();
            TextBlock Title = new TextBlock();
            Title.Text = "Connection Page";
            Title.FontSize = 30;

            Canvas.SetLeft(Title, 50);
            Canvas.SetTop(Title, 100);

            canva.Children.Add(Title);

            TextBlock infotop = new TextBlock();
            infotop.Text = "Select a device to connect to";
            infotop.FontSize = 15;
            
            Canvas.SetLeft(infotop, 50);
            Canvas.SetTop(infotop, 150);

            canva.Children.Add(infotop);
            #region list handling
            deviceslist.Width = 200;
            deviceslist.Height = 500;
            deviceslist.SelectedIndex = 0;
            

            Canvas.SetLeft(deviceslist, 50);
            Canvas.SetTop(deviceslist, 200);

            // add bluetooth devices to list
            List<string> deviceNames = BLEhandler.nearbyBluetootDevices();


            foreach (string name in deviceNames)
            {
                deviceslist.Items.Add(name);
            }
            
            canva.Children.Add(deviceslist);


            if (deviceNames.Count == 0)
            {
                TextBlock noDevices = new TextBlock();
                noDevices.Text = "No devices found";
                noDevices.FontSize = 15;

                Canvas.SetLeft(noDevices, 50);
                Canvas.SetTop(noDevices, 200);
                canva.Children.Add(noDevices);
            }
            #endregion


            #region buttons
            Button connectbbutton = new Button();
            connectbbutton.Content = "Connect";
            connectbbutton.Style = (Style)Application.Current.Resources["ButtonRevealStyle"];
            connectbbutton.Click += connectbuttonclick;

            Canvas.SetLeft(connectbbutton, 50);
            Canvas.SetTop(connectbbutton, 500);

            canva.Children.Add(connectbbutton);
            #endregion


        }

        
    }
}
    
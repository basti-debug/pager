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

namespace PagerClient
{
    public class PageCreator
    {
        #region variables

        // standard grid 
        Grid aGrid = new Grid();
        


        // Theme

        public static string theme = "Default";

        // for UI navigation
        private Frame usedcurrentframe = null;
        private Canvas usedcanvas = null;
        private Window usedwindow = null;
        private NavigationView nvham = null;

        private SolidColorBrush accent = new SolidColorBrush((Windows.UI.Color)Application.Current.Resources["SystemAccentColor"]);

        #endregion

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
            lstmsg.Text = "Last Message: ";
            lstmsg.FontSize = 15;


            ListView msglist = new ListView();
            
            //msglist.Background = accent;

            msglist.Items.Add("Message 1");
            msglist.Items.Add("Message 2");
            msglist.Width = 500;

            Canvas.SetLeft(msglist, 50);
            Canvas.SetTop(msglist, 150);

            currentframe.Children.Add(msglist);


            // Moving Objects inside the Canvas
            Canvas.SetLeft(Title, 50);
            Canvas.SetTop(Title, 50);

            Canvas.SetLeft(info1, 50);
            Canvas.SetTop(info1, 100);

            currentframe.Children.Add(Title);
            currentframe.Children.Add(info1);

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

        public void displayConnectionPage(Canvas canva)
        {
            canva.Children.Clear();
            TextBlock Title = new TextBlock();
            Title.Text = "Connection Page";
            Title.FontSize = 30;

            Canvas.SetLeft(Title, 50);
            Canvas.SetTop(Title, 100);

            canva.Children.Add(Title);

            ListView list = new ListView();
            list.Width = 200;
            list.Height = 500;
            list.SelectedIndex = 0;

            Canvas.SetLeft(list, 50);
            Canvas.SetTop(list, 150);

            // add bluetooth devices to list



            
            List<string> nearby = BLEhandler.nearbyBLE();   
            foreach (var item in nearby)
            {
                list.Items.Add(item);
            }
            
            canva.Children.Add(list);


            
        }
    }
}

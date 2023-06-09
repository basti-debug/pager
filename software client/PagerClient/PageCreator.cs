using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Shapes;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage;

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
        #endregion
        
        public void displayMainPage(Frame currentframe, Grid grid, Window window)
        {
            usedwindow = window;
            usedcurrentframe = currentframe;


            aGrid = grid;

            TextBlock Title = new TextBlock();
            Title.Text = "Welcome to LTGarlic";
            Title.FontSize = 30;

            TextBlock info1 = new TextBlock();
            info1.Text = "Version 2.6";
            info1.FontSize = 15;

            // Moving Objects inside the Canvas
            Canvas.SetLeft(Title, 50);
            Canvas.SetTop(Title, 100);

            Canvas.SetLeft(info1, 50);
            Canvas.SetTop(info1, 150);

            grid.Children.Add(Title);
            grid.Children.Add(info1);

        }

        public void displaySettingsPage(Frame currentframe, Grid grid)
        {
            TextBlock Title = new TextBlock();
            Title.Text = "Settings";
            Title.FontSize = 30;

            Canvas.SetLeft(Title, 50);
            Canvas.SetTop(Title, 100);

            grid.Children.Add(Title);
        }

        public void displayConnectionPage(Frame currentframe, Grid grid, Window window)
        {
            TextBlock Title = new TextBlock();
            Title.Text = "Connection Page";
            Title.FontSize = 30;

            Canvas.SetLeft(Title, 50);
            Canvas.SetTop(Title, 100);

            grid.Children.Add(Title);

            ListView list = new ListView();
            list.Width = 50;
            list.Height = 100;
            list.SelectedIndex = 0;

            var items = new[]
            {
                "Pager 1 ",
                "Pager 2"
            };

            list.Items.Add(items);

            grid.Children.Add(list);
        }
    }
}

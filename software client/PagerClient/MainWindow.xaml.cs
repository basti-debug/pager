// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Composition.SystemBackdrops;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Data;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Navigation;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using WinRT;
using System.Runtime.InteropServices; // For DllImport
using System.Reflection.PortableExecutable;





// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace PagerClient
{
    
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        PageCreator newpage = new PageCreator();


        WindowsSystemDispatcherQueueHelper m_wsdqHelper; // See separate sample below for implementation
        Microsoft.UI.Composition.SystemBackdrops.MicaController m_micaController;
        Microsoft.UI.Composition.SystemBackdrops.SystemBackdropConfiguration m_configurationSource;
        public MainWindow()
        {
            this.InitializeComponent();
            TrySetMicaBackdrop();
            MainPagerWindow.ExtendsContentIntoTitleBar = true;  // enable custom titlebar
            MainPagerWindow.SetTitleBar(PagerAppTitleBar);      // set user ui element as titlebar
            
            newpage.displayMainPage(contentCanvas);
            
            nvHamburgerleft.SelectionChanged += NvSample_SelectionChanged;

            #region switch pages 
            void NvSample_SelectionChanged(NavigationView sender, NavigationViewSelectionChangedEventArgs args)
            {
                var item = args.SelectedItem as NavigationViewItem;



                if (item.Tag != null && item.Tag.Equals("MainItem"))
                {
                    newpage.displayMainPage(contentCanvas);
                }
                if (item.Tag != null && item.Tag.Equals("Settings"))
                {
                    newpage.displaySettingsPage(contentCanvas);
                }
                if (item.Tag != null && item.Tag.Equals("connectpager"))
                {
                    newpage.displayConnectionPage(contentCanvas);
                }

            }

            #endregion

        }

        class WindowsSystemDispatcherQueueHelper
        {
            [StructLayout(LayoutKind.Sequential)]
            struct DispatcherQueueOptions
            {
                internal int dwSize;
                internal int threadType;
                internal int apartmentType;
            }

            [DllImport("CoreMessaging.dll")]
            private static extern int CreateDispatcherQueueController([In] DispatcherQueueOptions options, [In, Out, MarshalAs(UnmanagedType.IUnknown)] ref object dispatcherQueueController);

            object m_dispatcherQueueController = null;
            public void EnsureWindowsSystemDispatcherQueueController()
            {
                if (Windows.System.DispatcherQueue.GetForCurrentThread() != null)
                {
                    // one already exists, so we'll just use it.
                    return;
                }

                if (m_dispatcherQueueController == null)
                {
                    DispatcherQueueOptions options;
                    options.dwSize = Marshal.SizeOf(typeof(DispatcherQueueOptions));
                    options.threadType = 2;    // DQTYPE_THREAD_CURRENT
                    options.apartmentType = 2; // DQTAT_COM_STA

                    CreateDispatcherQueueController(options, ref m_dispatcherQueueController);
                }
            }
        }



        bool TrySetMicaBackdrop()
        {
            if (Microsoft.UI.Composition.SystemBackdrops.MicaController.IsSupported())
            {
                m_wsdqHelper = new WindowsSystemDispatcherQueueHelper();
                m_wsdqHelper.EnsureWindowsSystemDispatcherQueueController();

                // Hooking up the policy object
                m_configurationSource = new Microsoft.UI.Composition.SystemBackdrops.SystemBackdropConfiguration();
                this.Activated += Window_Activated;
                this.Closed += Window_Closed;
                ((FrameworkElement)this.Content).ActualThemeChanged += Window_ThemeChanged;

                // Initial configuration state.
                m_configurationSource.IsInputActive = true;
                SetConfigurationSourceTheme();

                m_micaController = new Microsoft.UI.Composition.SystemBackdrops.MicaController();

                // Enable the system backdrop.
                // Note: Be sure to have "using WinRT;" to support the Window.As<...>() call.
                m_micaController.AddSystemBackdropTarget(this.As<Microsoft.UI.Composition.ICompositionSupportsSystemBackdrop>());
                m_micaController.SetSystemBackdropConfiguration(m_configurationSource);
                return true; // succeeded
            }

            return false; // Mica is not supported on this system
        }

        private void Window_Activated(object sender, WindowActivatedEventArgs args)
        {
            m_configurationSource.IsInputActive = args.WindowActivationState != WindowActivationState.Deactivated;
        }

        private void Window_Closed(object sender, WindowEventArgs args)
        {
            // Make sure any Mica/Acrylic controller is disposed so it doesn't try to
            // use this closed window.
            if (m_micaController != null)
            {
                m_micaController.Dispose();
                m_micaController = null;
            }
            this.Activated -= Window_Activated;
            m_configurationSource = null;
        }

        private void Window_ThemeChanged(FrameworkElement sender, object args)
        {
            if (m_configurationSource != null)
            {
                SetConfigurationSourceTheme();
            }
        }

        private void SetConfigurationSourceTheme()
        {
            switch (((FrameworkElement)this.Content).ActualTheme)
            {
                case ElementTheme.Dark: m_configurationSource.Theme = Microsoft.UI.Composition.SystemBackdrops.SystemBackdropTheme.Dark; break;
                case ElementTheme.Light: m_configurationSource.Theme = Microsoft.UI.Composition.SystemBackdrops.SystemBackdropTheme.Light; break;
                case ElementTheme.Default: m_configurationSource.Theme = Microsoft.UI.Composition.SystemBackdrops.SystemBackdropTheme.Default; break;
            }
        }






    }    
}

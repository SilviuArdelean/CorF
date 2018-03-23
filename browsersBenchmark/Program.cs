using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Xml.Linq;

namespace browsersBenchmark
{

    class RunBrowsersInParallel
    {
        private List<string> listBrowsers = new List<string>();
        private List<string> listURLs = new List<string>();

        void BuildBrowsersList()
        {
            listBrowsers.Clear();

            using (RegistryKey hklm = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32))
            {
                RegistryKey webClientsRootKey = hklm.OpenSubKey(@"SOFTWARE\Clients\StartMenuInternet");
                if (webClientsRootKey != null)
                {
                    foreach (var subKeyName in webClientsRootKey.GetSubKeyNames())
                    {
                        if (webClientsRootKey.OpenSubKey(subKeyName) != null)
                        {
                            if (webClientsRootKey.OpenSubKey(subKeyName).OpenSubKey("shell") != null)
                            {
                                if (webClientsRootKey.OpenSubKey(subKeyName).OpenSubKey("shell").OpenSubKey("open") != null)
                                {
                                    if (webClientsRootKey.OpenSubKey(subKeyName).OpenSubKey("shell").OpenSubKey("open").OpenSubKey("command") != null)
                                    {
                                        string commandLineUri = (string)webClientsRootKey.OpenSubKey(subKeyName).OpenSubKey("shell").OpenSubKey("open").OpenSubKey("command").GetValue(null);

                                        commandLineUri = commandLineUri.Replace("\"", string.Empty);

                                        listBrowsers.Add(commandLineUri);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

 

        void BuildURLsList()
        {
            XDocument doc = XDocument.Load("URLs.xml");

            var url_results = doc.Descendants("urls_to_browse").Descendants("url");
            foreach (var url in url_results)
            {
                listURLs.Add(url.Value);
            }

            if (listURLs.Count == 0)
            {
                listURLs.Add("https://www.google.com</url>");
                listURLs.Add("https://www.bing.com</url>");
                listURLs.Add("http://silviuardelean.ro");
            }
        }

        void RunPSA(string fileName)
        {
            var procPSA = new Process();
            procPSA.StartInfo.FileName = "psa.exe";
            procPSA.StartInfo.Arguments = "-o " + fileName;
            procPSA.StartInfo.UseShellExecute = false;
            procPSA.StartInfo.RedirectStandardOutput = false;
            procPSA.Start();
        }

        bool RunSeparateProcesses(string fileName)
        {
            return  (StringUtils.ContainsNoCase(fileName, "iexplore", StringComparison.OrdinalIgnoreCase) ||  
                        StringUtils.ContainsNoCase(fileName, "safari", StringComparison.OrdinalIgnoreCase));
        }
        void ExecuteProcess(string execPath, string fileName)
        {
            if (StringUtils.ContainsNoCase(fileName, "fileName", StringComparison.OrdinalIgnoreCase))
            {
                // kill other instances if they exists using SysInternals tool
                if (File.Exists("pskill.exe"))
                {
                    Process.Start("pskill.exe " + fileName);
                }
            }

            if (RunSeparateProcesses(fileName))
            {
                foreach (var url in listURLs)
                {
                    var procBrowser = new Process();
                    procBrowser.StartInfo.FileName = execPath;
                    procBrowser.StartInfo.Arguments = url;
                    procBrowser.StartInfo.UseShellExecute = false;
                    procBrowser.StartInfo.RedirectStandardOutput = true;
                    procBrowser.Start();

                    Thread.Sleep(1000);     // seems Safari needs some time
                }
            }
            else
            {
                // any decent browser
                var procBrowser = new Process();
                procBrowser.StartInfo.FileName = execPath;

                foreach (var url in listURLs)
                    procBrowser.StartInfo.Arguments += url + " ";

                procBrowser.StartInfo.UseShellExecute = false;
                procBrowser.StartInfo.RedirectStandardOutput = true;
                procBrowser.Start();
            }

            Console.WriteLine("RUN - " + fileName.ToUpper() + " ---> " + execPath);
            Console.ReadKey();

            Console.WriteLine("[---   Memory usage results using psa.exe ----]");
            RunPSA(fileName);
            Console.WriteLine("-----------------------------------------------");

            Console.ReadKey();
        }

        void RunBenchmark()
        {
            foreach (var browserPath in listBrowsers)
            {
                string fileNameOnly = Path.GetFileName(browserPath);
                fileNameOnly = fileNameOnly.Replace(@".exe", string.Empty); 

                if (StringUtils.ContainsNoCase(browserPath, "Opera", StringComparison.OrdinalIgnoreCase))  // latest Opera versions are launched by Launcher.exe
                    fileNameOnly = "opera";



                ExecuteProcess(browserPath, fileNameOnly);
            }

            if (IsWindows10())
            {
                foreach (var url in listURLs)
                {
                    //https://stackoverflow.com/a/33625521
                    //Edge process is "recycled", therefore no new process is returned.
                    Process.Start("microsoft-edge:" + url);

                    //We need to find the most recent MicrosoftEdgeCP process that is active
                    var edgeProcessList = Process.GetProcessesByName("MicrosoftEdgeCP");
                    Process newestEdgeProcess = null;

                    foreach (var theprocess in edgeProcessList)
                    {
                        if (newestEdgeProcess == null || theprocess.StartTime > newestEdgeProcess.StartTime)
                        {
                            newestEdgeProcess = theprocess;
                        }
                    }
                }

                Console.WriteLine("RUN - Microsoft Edge");
                Console.ReadKey();

                Console.WriteLine("[---   Memory usage results using psa.exe ----]");
                RunPSA("Edge");
                Console.WriteLine("-----------------------------------------------");
            }
        }

        static bool IsWindows10()       // dirty way to do it for limited proposes 
        {
            var reg = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows NT\CurrentVersion");

            string productName = (string)reg.GetValue("ProductName");

            return productName.StartsWith("Windows 10");
        }

        static void Main(string[] args)
        {
            if (!File.Exists("psa.exe"))
            {
                Console.WriteLine("psa.exe benchmark file not available in current path");
                Console.WriteLine("add psa.exe (Process Status Analysis) file in current path and try again");
                Console.WriteLine("psa.exe sources - Process Status Analysis - https://github.com/SlyMaximus/psa");
                return;
            }

            RunBrowsersInParallel run = new RunBrowsersInParallel();

            run.BuildBrowsersList();
            run.BuildURLsList();
            run.ShowDetectedBrowsers();
           
            run.RunBenchmark();
        }

        void ShowDetectedBrowsers()
        {
            Console.WriteLine("----- Detected Browsers Path ------");

            foreach (var browserPath in listBrowsers)
            {
                Console.WriteLine(browserPath);
            }

            Console.WriteLine("--------------------------------------");

            Console.WriteLine("------- URLs to be accessed --------");
            foreach (var url in listURLs)
            {
                Console.WriteLine(url);
            }

            Console.WriteLine("_______________________________________");

        }

    }

    public static class StringUtils
    {
        public static bool ContainsNoCase(this string source, string toCheck, StringComparison comp)
        {
            return source != null && toCheck != null && source.IndexOf(toCheck, comp) >= 0;
        }
    }
}

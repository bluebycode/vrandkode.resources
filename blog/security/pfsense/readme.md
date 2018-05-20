
References: https://mop.cv.uma.es/pluginfile.php/521577/mod_resource/content/10/DCSSR-StudentsGuide2018.pdf

1. Install pfsense image from here:

https://atxfiles.pfsense.org/mirror/downloads/

or 

```wget https://atxfiles.pfsense.org/mirror/downloads/pfSense-CE-2.3.5-RELEASE-2g-i386-nanobsd.img.gz```

2. Add pfSense 2.3.5 from Available appliances and select the image downloaded (gns3 will detect it automatically or not)

3. Settings > Number of interfaces set to 2

4. Link: em0 (from router) and em1 (from internet)

5. Console > Wait for setup and type the following options:

```
Welcome to pfSense 2.3.5-RELEASE (amd64 nanobsd) on pfSense ***

 WAN (wan)       -> em0        -> v4/DHCP4: 172.16.221.125/24
 LAN (lan)       -> em1        -> v4: 192.168.1.1/24

 0) Logout (SSH only)                  9) pfTop
 1) Assign Interfaces                 10) Filter Logs
 2) Set interface(s) IP address       11) Restart webConfigurator
 3) Reset webConfigurator password    12) PHP shell + pfSense tools
 4) Reset to factory defaults         13) Update from console
 5) Reboot system                     14) Enable Secure Shell (sshd)
 6) Halt system                       15) Restore recent configuration
 7) Ping host                         16) Restart PHP-FPM
 8) Shell
  

>>>>>>> Enter an option: 1


Valid interfaces are:

em0    52:2f:3c:f8:dc:00   (up) Intel(R) PRO/1000 Legacy Network Connection 1.1.
em1    52:2f:3c:f8:dc:01   (up) Intel(R) PRO/1000 Legacy Network Connection 1.1.

Do VLANs need to be set up first?
If VLANs will not be used, or only for optional interfaces, it is typical to
say no here and use the webConfigurator to configure VLANs later, if required.

>>>>>>> Should VLANs be set up now [y|n]? n

If the names of the interfaces are not known, auto-detection can
be used instead. To use auto-detection, please disconnect all
interfaces before pressing 'a' to begin the process.

Enter the WAN interface name or 'a' for auto-detection 
>>>>>>> (em0 em1 or a): em0

Enter the LAN interface name or 'a' for auto-detection 
NOTE: this enables full Firewalling/NAT mode.
>>>>>>> (em1 a or nothing if finished): em1

Enter the Optional 1 interface name or 'a' for auto-detection
>>>>>>>  ( a or nothing if finished): ENTER

The interfaces will be assigned as follows:

WAN  -> em0
LAN  -> em1

>>>>>>> Do you want to proceed [y|n]? y

Writing configuration...done.
One moment while the settings are reloading... done!
embedded (unknown) - Netgate Device ID: c656c6f5c3581acf75e2

*** Welcome to pfSense 2.3.5-RELEASE (amd64 nanobsd) on pfSense ***

 WAN (wan)       -> em0        -> v4/DHCP4: 172.16.221.125/24
 LAN (lan)       -> em1        -> v4: 192.168.1.1/24

DONE
```
6. Go SHELL: option 8 and ping www.google.com

````

Enter an option: 8

[2.3.5-RELEASE][root@pfSense.localdomain]/root: ping www.google.com
PING www.google.com (216.58.211.36): 56 data bytes
64 bytes from 216.58.211.36: icmp_seq=0 ttl=54 time=20.650 ms
64 bytes from 216.58.211.36: icmp_seq=1 ttl=54 time=22.330 ms
64 bytes from 216.58.211.36: icmp_seq=2 ttl=54 time=21.394 ms
64 bytes from 216.58.211.36: icmp_seq=3 ttl=54 time=23.762 ms
^C
--- www.google.com ping statistics ---
4 packets transmitted, 4 packets received, 0.0% packet loss
round-trip min/avg/max/stddev = 20.650/22.034/23.762/1.162 ms
[2.3.5-RELEASE][root@pfSense.localdomain]/root: 

```

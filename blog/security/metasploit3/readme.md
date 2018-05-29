

References: https://zero-day.io/metasploitable3/

1. Installing vagrant from https://www.vagrantup.com/downloads.html

First you need to download and install Vagrant. Pick your flavor (x86 or x64) here and install. Reboot afterwards.

Additionally, you need to install the Vagrant Reload Plugin. Simply run the following command in the Windows Command Line:

vagrant plugin install vagrant-reload

2. Download the metasploit 3 project using github zip https://github.com/rapid7/metasploitable3/

3. Download and install Packer https://www.packer.io/downloads.html

4. Move packer script at the same level of metasploit unzipped

5. Run the install script. To run the install script, start Powershell as administrator, change directory to the unzipped Metasploit 3 directory and run the build_win2008.ps1 script. This will take approx 15-20 minutes.

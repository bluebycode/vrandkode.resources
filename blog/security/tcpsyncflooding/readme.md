# TCP SYN FLOODING ATTACK
Perform a TCP SYN flooding attack using “metasploit” (in Kali linux, the student may
follow the steps in the student’s Guide). Use Wireshark to show the answers to the SYN requests and use the utilities
for diagnosis in the Mikrotik router.

1. Preparation

1.1. Connect tru router and check blacklist is empty
```
admin@MikroTik]> /ip firewall address-list print
Flags: X - disabled, D - dynamic 
 #   LIST            
 ```
 
1.2. Run *Kali* and check if the port 80 is opened in the gateway, using Sparta applicacion

![](4_sparta_port80.png)

1.3. Try browse the admin page from router.

![](5_firefox_router_web.png)


2. Attacker side

2.1 Perfoms a TCP SYN flooding attack using metasploit program
```
> use auxiliary/scanner/portscan/tcp
> set RHOST 192.10.1.254
> 
```



3. Defensive side

From engineer side, we need to expose a diagnostic during the possible attack, we ought to mark  a check list, to ensure us we got a attack, and in that case apply a countermeasure with different working arounds:

3.1 Are we under attack?
Go to router admin page and ensure:
* 







 
 
 
 

EXERCISE 1. INITIAL QUESTIONS

The student must answer the following questions about both networks, before performing any test:

*1. Is there any mechanism that could be considered a security mechanism in the network environments? Which
one? Why?*

La idea es que un mecanismo de seguridad pueda evitar or mitigar directa o indirectamente la acción
de un intruso mediante un ataque pasivo o activo. 

A nivel físico, no podemos realizar ninguna mención ya que no disponemos
de la información para los entornos de red: protección geográfica, física de los dispositivos (armarios primarios, secundarios),
acceso restringido a los cables para evitar el riesgo de ser intervenido con invasive taps, which are most suitable for passive attacks.

A nivel lógico, incluiremos como mecanismos de seguridad el conjunto de elementos o dispositivos su disposición en la red, 
y que permiten separaciones a nivel de enlace OSI de redes y subredes; permitiendo añadir ofuscaciçon y evitar ataques pasivos de 
descubrimiento en la intranet red. 

Al añadir el elemento de acceso exterior, estamos expuesto a otra posible lista de requerimientos para evitar que nuestar red sea 
vulnerable esta vez desde fuera de la red interna. Para ello, elementos que permitan rechazar determinados accesos, o la
simple disposición de los dispositivos de enroutamientos que acoten y aisle las redes, tal y como mencionamos anteriormente.

2. What configuration do you think is most appropriate? Why?

Tanto la primera y la segunda configuración estan dispuestas a los mismos ataques desde Internet sobre el router externo, por lo 
que prima la posible vulnerabilidad de la configuración de éste: firewall. Si el intruso consigue acceder debido a algun tipo de 
vulnerabilidad de leak of configuration por ejemplo, puede realizar un ataque pasivo sobre la red, a partir de aqui regirá la
configuración de la red interna.

Si nos basamos en un posible ataque mediante Kali conectado a ambas configuraciones, tendremos en cuenta el numero de pasos 
que se necesitaría para realizar un ataque a la otra parte de la red (la máquina vulnerable), para ello nos basaremos:
- Calidad de la configuración de la red donde se encuentra alojada
- Visibilidad de la subred desde la red atacante.

Con respecto a la calidad de la configuración, el nivel de autenticacion y el acceso a los dispositivos de eroutamiento,
en la segunda configuración no están habilitados por defectos el acceso, por lo cual se puede limitar los ataques directos.
A nivel de definidicçon de vlans, la primera configuración esta expuesta a ser vlan spoofeada al estar vinculada la disposicin
de las vlanas a nivel del switch1, y el acceso trunk expuesto a las dos vlans.

http://www.ciscopress.com/articles/article.asp?p=1626588&seqNum=2
??


3. List the attacks that you think could be made and between what network elements. Provide examples
naming the network elements.

Primer escenario

main Router - acceso con contraseña por defecto desde afuera, gateway por defecto



Segundo escenario



----

EXERCISE 2. NETWORK RECOGNITION
Choose one (or some) of the applications for information gathering in Kali to identify the elements in the network
SWITCHROUT-Holes. The student’s Guide includes examples of the use of some of these tools.
The student must provide relevant captures of the process and answer the following questions:

1. What tools have you chosen and why?

From kali,

Network scanning
  Sparta.  network sc







2. How many network scans did you need to identify all the possible targets of the attack?


3. Show the results of the network scan that allows to find the machine with vulnerabilities and verifies that
it is possible to visualize the web page running in the virtual machine.


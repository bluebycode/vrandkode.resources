# Seguridad en MQTT

Como podemos incluir seguridad en MQTT:

* Capa de Network level: VPN
* Capa de transporte: SSL/TLS
* Capa de aplicación: Access token => OpenAuth2

## Referencias: https://es.slideshare.net/AnthonyChow5/mqtt-security

## SSL/TLS

SSL – Secure Socket Layer (older standard) o Version 2 and version 3
TLS – Transport Layer Security (newer standard) o Version 1.1, 1.2 and 1.3
Asymmetric encryption o Private Key and Public key
Symmetric encryption o Symmetric key
Hashing
Digital Certificate – e.g. X.509

* SSL Handshake
![](https://www.ibm.com/support/knowledgecenter/en/SSFKSJ_7.1.0/com.ibm.mq.doc/sy10660a.gif)

* SSL – X.509 Digital Certificate

![](https://image.slidesharecdn.com/mqttsecurity-180403224936/95/mqtt-security-15-638.jpg?cb=1522795890)

## Añadiendo Authentication/Authorisation con OpenAuth2

Referencias: http://pzf.fremantle.org/2013/11/using-oauth-20-with-mqtt.html


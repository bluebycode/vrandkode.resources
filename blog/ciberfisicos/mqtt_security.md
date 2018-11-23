# Seguridad en MQTT

Como podemos incluir seguridad en MQTT:

* Capa de Network level: VPN
* Capa de transporte: SSL/TLS
* Capa de aplicación: Access token => OpenAuth2

## Referencias: https://es.slideshare.net/AnthonyChow5/mqtt-security

## Autenticación basica con usuario/password 

Username and PasswordUsername and Password
```
$ mosquitto_passwd –U <password-file>
$ mosquitto_passwd –c <password-file> <user> {password}
```

Edit /etc/mosquitto.conf:
```
allow_anonymous false
password_file /etc/mosquitto/<password-file>
```

## Access control

ACL – Access Control ListACL – Access Control List  /etc/mosquitto/mosquitto.conf  /etc/mosquitto/conf.d/default.conf  Add this line:  acl_file /etc/mosquitto/<acl-file>
29. Sample ACL file forSample ACL file for MosquittoMosquitto Source: https://jaimyn.com.au/mqtt-use-acls-multiple-user-accounts/ # Give Home user1 full access to everything user user1 topic readwrite # # Allow the user2 to read/write to test/# and stat/# user user2 topic readwrite test/# topic readwrite stat/# # Allows user3 to read/write to the sensor topics user user3 topic cmnd/sensor/# topic stat/sensor/#
  
## SSL/TLS

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

openssl genrsa -out ca.key 2048  openssl req -new -x509 -days365 -key ca.key -out ca.crt  openssl genrsa -out serv.key 2048  openssl req -new -key serv.key -out serv.csr  openssl x509 -req -in serv.csr -CA mosq-ca.crt -CAkey ca.key -CAcreateserial -out serv.crt -days 365 -sha256  Add this line:  Listener 8883  cafile /home/mosquitto/ca.crt  certfile /home/mosquitto/serv.crt  keyfile /home/mosquitto/serv.key

## Añadiendo Authentication/Authorisation mediante un tercero OpenAuth2

Referencias: 

* Oauth: http://pzf.fremantle.org/2013/11/using-oauth-20-with-mqtt.html

* Mosquitto Plugin: https://github.com/jpmens/mosquitto-auth-plug


rdrd Party – OAuth2/Auth0Party – OAuth2/Auth0  https://auth0.com/docs/integrations/authenticating-devices-using-mqtt  openssl genrsa -out serv.key 204  openssl req -new -key serv.key -out serv.csr  openssl x509 -req -in serv.csr -CA mosq-ca.crt -CAkey ca.key -CAcreateserial -out serv.crt -days 365 -sha256  Add this line:  Listener 8883  cafile /home/mosquitto/ca.crt  certfile /home/mosquitto/serv.crt  keyfile /home/mosquitto/serv.key

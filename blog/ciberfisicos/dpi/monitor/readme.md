

* Levantamos la infrastructura con ayuda de Docker con ElasticSearch y Grafana.

```
$ docker compose up
```

* Creamos los indices con los formatos de los campos necesarios

```
#!/bin/bash
host=${1:-"192.168.1.74"}
curl -XDELETE http://$host:9200/rawdata
curl -XPUT http://$host:9200/rawdata
curl -XPUT http://$host:9200/rawdata/_mapping/ts -d '{ 
	"properties": 
	{ "@timestamp" : 
		{ "type": "date","format": "epoch_millis||epoch_second"}
	}
}' -H 'content-type:application/json;'
curl -XGET http://$host:9200/rawdata

```

* Ya podemos conectar y enviar información desde el DPI (importante crear los índices antes)

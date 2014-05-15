>empleado.txt
>estacion.txt
>jefeEstacion.txt
>administrador.txt


while read line; do 
	echo "$line" | grep 'empleado\.cpp\.[0-9]*' >> empleado.txt
	echo "$line" | grep 'estacion\.cpp' >> estacion.txt
	echo "$line" | grep 'jefeEstacion\.cpp' >> jefeEstacion.txt
	echo "$line" | grep 'administrador\.cpp' >> administrador.txt
	if [ "$1" = "-v" ]; then echo $line; fi;
done

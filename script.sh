#!/bin/bash

esperar_tecla(){
	echo -e "\nPresione una tecla para continuar..."
	read -n 1 -s -r -p ""
}

sudo chmod -R 777 /*

sudo rm ./punto_3/test_punto3.img
sudo cp ./punto_3/aux/test_punto3.img ./punto_3/

sudo rm ./punto_4/test_punto4.img
sudo cp ./punto_4/aux/test_punto4.img ./punto_4/ #Reestablece los .img que se han usado
sudo mkdir /gp_3 #Crea el punto de montaje


echo "-Grupo 3:Crevatin,Fandiño,Mendoza,Palacio-"
echo -e "		|Punto 2:Mostrando el MBR|\n"
./punto_2/read_mbr
esperar_tecla

echo "		|Punto 3:Tabla de archivos|"
echo "		|Mostrando los archivos del Filesystem|"
./punto-3/read_fs
esperar_tecla

echo "		|Montando imagen en /gp_3/ ,por favor, aguarde|"
sudo mount ./punto_3/test_punto3.img /gp_3 -o loop,umask=000
sleep 3s
echo "		|Creando archivo test.txt en /gp_3/|"
touch /gp_3/test.txt
sleep 3s
echo "		|Mostrando nuevamente los archivos|"
#Espera ese tiempo debido a que  el .img tarda en cargar el nuevo archivo
sleep 5s
sudo umount /gp_3/
sudo mount ./punto_3/test_punto3.img /gp_3 -o loop,umask=000
echo "			    |Cargando|"
sleep 5s
cd ./punto_3/
./read_fs_punto3
echo "		|Borrando archivo test.txt en /gp_3/|"
sudo rm /gp_3/test.txt
sleep 3s
echo "		|Mostrando nuevamente los archivos|"
sudo umount /gp_3/
sudo mount ./test_punto3.img /gp_3 -o loop,umask=000
sleep 5
./read_fs_punto3
cd ..
sudo umount /gp_3/
esperar_tecla
echo "		|Punto 4:Leyendo archivos|"
echo "		|Montando archivo test_punto4.img|"
cd ./punto_4/
sudo mount ./test_punto4.img /gp_3 -o loop,umask=000
sleep 5
echo "		|Creando archivo lapapa.txt|"
sudo touch /gp_3/lapapa.txt
echo "Bienvenidos, soy la papa" > /gp_3/lapapa.txt
echo "		|Mostrando el contenido de los archivos|"
sudo umount /gp_3/
sudo mount ./test_punto4.img /gp_3 -o loop,umask=000
./read_files_content
echo "		|Eliminando archivo lapapa.txt|"
sudo rm /gp_3/lapapa.txt
sleep 3s
sudo umount /gp_3/
sudo mount ./test_punto4.img /gp_3 -o loop,umask=000
echo "		|Mostrando los archivos|"
./read_files
echo "		|Ejecutando recuperador de archivos 2.0|"
sleep 2s
sudo umount /gp_3/
sudo mount ./test_punto4.img /gp_3 -o loop,umask=000
sudo ./recuperador
echo "		|Mostrando el contenido de los archivos|"
sudo umount /gp_3/
sudo mount ./test_punto4.img /gp_3 -o loop,umask=000
./read_files_content
echo "Script finalizado"


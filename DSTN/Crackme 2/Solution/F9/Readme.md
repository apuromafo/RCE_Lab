# Solucion VM Aces_v3 por DSTN

Esta carpeta de RCE_Lab muestra los archivos y recursos que utilicé para solucionar el Crackme Aces_v3 de DSTN. 
Este crackme se basa en una máquina virtual que basa su ejecución de instrucciones en un handler de excepciones (parecido a Nanomites de Armadillo), cada una de las instrucciones que realiza pasará por este handler de excepciones.
Para resolver la VM lo primero será saber cada una de las posibles instrucciones que ejecuta la VM, entre ellas nos encontramos saltos, llamadas a funciones, funciones de API y ejecución de instrucciones simples pertenecientes al programa original.
Una vez conocemos esto, es posible ir debuggeando sabiendo qué estamos ejecutando.

Tras conocer un poco el programa, llegaremos entonces a la comparación y al salto que hay que parchear para poder "crackear" el programa, este salto no será una instrucción sencilla de ensamblador, sino que esta será una instrucción perteneciente al bytecode de la VM.

## Archivos solución

* <a href="https://github.com/Fare9/RCE_Lab/blob/master/DSTN/Crackme%202/Solution/F9/Aces_v3%20-%20cracked%20by%20F9.exe">Aces_v3_cracked_by_F9.exe</a>: este es el ejecutable modificado respecto al original.
* <a href="https://github.com/Fare9/RCE_Lab/blob/master/DSTN/Crackme%202/Solution/F9/CrackedF9.JPG">CrackedF9.JPG</a>: imagen de ejemplo del programa crackeado.
* <a href="https://github.com/Fare9/RCE_Lab/blob/master/DSTN/Crackme%202/Solution/F9/Aces_v3.idb">Aces_v3.idb</a>: idb de la VM comentado.
* <a href="https://github.com/Fare9/RCE_Lab/blob/master/DSTN/Crackme%202/Solution/F9/gustavo.dd32">gustavo.dd32</a>: dd32 para x32dbg con etiquetas para debugging de la VM original.
* <a href="https://github.com/Fare9/RCE_Lab/blob/master/DSTN/Crackme%202/Solution/F9/executeUntilWorks.py">executeUntilWorks.py</a>: script idapython para poder ejecutar el debugger de IDA hasta que VirtualAlloc sea correcto.
* <a href="https://github.com/Fare9/RCE_Lab/blob/master/DSTN/Crackme%202/Solution/F9/setAsStructure.py">setAsStructure.py</a>: script de idapython para establecer algunas estructuras de la VM.
* <a href="https://github.com/Fare9/RCE_Lab/blob/master/DSTN/Crackme%202/Solution/F9/Solucion.txt">Solucion.txt</a>: archivo con la explicación de la solución.
* <a href="https://github.com/Fare9/RCE_Lab/tree/master/DSTN/Crackme%202/Solution/F9/GusVMUnpacker">GusVMUnpacker</a>: proyecto de visual studio con un desvirtualizador y monitor de ejecución de la VM programado con TitanEngine.

Solución realizada por <a href="https://twitter.com/Farenain">Fare9</a> para CracksLatinoS.
Gracias a DSTN por realizar esta VM, he podido aprender bastante de este mundillo de la ofuscación del cual no tenía casi ni idea.

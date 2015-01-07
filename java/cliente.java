// Librerias ... a utilizar
import java.io.*;
import java.net.*;
import java.util.*;
import java.lang.Integer;

public class cliente {

// atributos de la clase
private static InetAddress host;
private static int PORT_Server;
private static String IP_Server;
private static DatagramSocket datagramSocket;
private static DatagramPacket inPacket, outPacket;
private static byte[] buffer;

// Metodo principal

public static void main(String args[]) throws IOException{
	   String l;
	   l = (args[1]);
        IP_Server=(args[0]); //ip del servidor
		PORT_Server = Integer.parseInt(l);//puerto del servidor
		
    try {

    	//host = InetAddress.getLocalHost();
			//host=InetAddress.getByName("192.168.1.1");

			host=InetAddress.getByName(IP_Server);

		} catch(UnknownHostException uhEx) {

    	System.out.println("ID de Host no Existente!");
      System.exit(1);

    } // fin del try catch

    
    accesaServer();

} // fin del metodo principal

private static void accesaServer() throws IOException{

    try {

    datagramSocket = new DatagramSocket(); //Paso 1... Crear Socket
	
    //creaci�n de un stream para entrada de teclado...
    //Scanner userEntry = new Scanner(System.in);
    BufferedReader a = new BufferedReader(new InputStreamReader(System.in));
    //String userEntry = a.readLine();
    String mensaje="", respuesta="";

    do {
            System.out.print("Introduzca su mensaje: ");

            //mensaje = userEntry.nextLine();
            mensaje = a.readLine();

            outPacket = new DatagramPacket( mensaje.getBytes(), mensaje.length(), host,PORT_Server); //Paso 2. - Crea Paquete de Solicitud

            datagramSocket.send(outPacket); //Paso 3...

            // recepcion de datos 
             buffer = new byte[256] ; // paso 2 - Crea bufer
            inPacket = new DatagramPacket(buffer , buffer.length); // paso 3 - Asigna Bufer al Socket
            datagramSocket.receive(inPacket); // paso 4 -   Escucha, hasta que le env�en algo


            InetAddress clientAddress = inPacket.getAddress(); // paso 5 - Clasifica informaci�n recibida del Cliente
            int clientPort = inPacket.getPort();
            int lRec = inPacket.getLength();
            respuesta = new String(inPacket.getData(),0,lRec); // paso 6 - Obtiene Mensaje recibido del cliente
            System.out.println("Mensaje recibido, Longitud: ["+lRec+"]: " + respuesta);
        
    } while (!mensaje.equals("CLOSE"));


    } catch(IOException ioEx) {

        ioEx.printStackTrace();

    } finally {

        System.out.println("\n* Cerrando conexion... *");
        datagramSocket.close(); //Paso 8. - Cierra Socket
    } // fin del try catch , finally

} // fin del metodo

} // fin de la clase

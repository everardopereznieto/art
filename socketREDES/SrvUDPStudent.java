// Librerias
import java.io.*;
import java.net.*;
import java.lang.Integer;


public class SrvUDPStudent {

    private static int PORT;
    private static DatagramSocket datagramSocket;
    private static DatagramPacket inPacket , outPacket;
    private static byte[] buffer;
    private static int numMensaje = 0;


    public static void main (String args[]) {
			String l;
			l = args[0];
			PORT =  Integer.parseInt(l);

      System.out.println("Abriendo Puerto "+PORT+" ....\n");

      try {

      	datagramSocket = new DatagramSocket(PORT); // Paso 1 - Crea socket

      } catch (SocketException sockEx) {
				System.out.println("Imposible conectrase al puerto ");
      	System.exit(1);
      }
    manejaCliente();
    } // fin del metodo principal


    private static void manejaCliente () {

        try {

            String mensajeIN , mensajeOUT;
            //int numMensaje = 0;


         do {
            buffer = new byte[256] ; // paso 2 - Crea bufer
            inPacket = new DatagramPacket(buffer , buffer.length); // paso 3 - Asigna Bufer al Socket
            datagramSocket.receive(inPacket); // paso 4	-	Escucha, hasta que le env�en algo


            InetAddress clientAddress = inPacket.getAddress(); // paso 5 - Clasifica informaci�n recibida del Cliente
            int clientPort = inPacket.getPort();
            int lRec = inPacket.getLength();
            mensajeIN = new String(inPacket.getData(),0,lRec); // paso 6 - Obtiene Mensaje recibido del cliente
            System.out.println("Mensaje recibido, Longitud: ["+lRec+"]: " + mensajeIN);
            

         } while(!mensajeIN.equals("CLOSE"));


         } catch (IOException ioEX) {

            ioEX.printStackTrace();

         } finally {

            System.out.println("\n* Cerrado conexion ...* ");
            datagramSocket.close(); // paso 9 - Cierra el Socket

         } // fin try , catch , finally


    } // fin del metodo


} // fin de la clase

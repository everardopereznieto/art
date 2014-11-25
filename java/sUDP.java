import java.net.*;
import java.io.*;
public class sUDP {
 public static void main(String argv[]) {

DatagramSocket socket;
 boolean fin = false;

try {
 //Creamos el socket
 socket = new DatagramSocket(6000);

byte[] mensaje_bytes = new byte[256];
 String mensaje ="";
 mensaje = new String(mensaje_bytes);
 String mensajeComp ="";

DatagramPacket paquete = new DatagramPacket(mensaje_bytes,256);
 DatagramPacket envpaquete = new DatagramPacket(mensaje_bytes,256);

int puerto;
 InetAddress address;
 byte[] mensaje2_bytes = new byte[256];

//Iniciamos el bucle
 do {
 // Recibimos el paquete
 socket.receive(paquete);
 // Lo formateamos
 mensaje = new String(mensaje_bytes).trim();
 // Lo mostramos por pantalla
 System.out.println(mensaje);
 //Obtenemos IP Y PUERTO
 puerto = paquete.getPort();
 address = paquete.getAddress();

 if (mensaje.startsWith("fin")) {
 mensajeComp="chauuuuuuu cliente";
 }

 if (mensaje.startsWith("hola")) {
 mensajeComp="hola cliente";
 }

 //formateamos el mensaje de salida
 mensaje2_bytes = mensajeComp.getBytes();

//Preparamos el paquete que queremos enviar
 envpaquete = new DatagramPacket(mensaje2_bytes,mensajeComp.length(),address,puerto);

// realizamos el envio
 socket.send(envpaquete);

} while (1>0);
 }
 catch (Exception e) {
 System.err.println(e.getMessage());
 System.exit(1);
 }
 }
}

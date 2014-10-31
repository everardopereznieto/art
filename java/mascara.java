import java.io.*;
import java.util.StringTokenizer;
public class mascara{
	public static void main(String[] args){
		int i;
	
	if(args.length<2 || args.length>2){
		System.out.println("uso:java mascara ip.ip.ip.ip mask.mask.mask.mask");
		}

	else {
		token(args[0]);
		token(args[1]);
		
		}	
	}
	
	/*int[] token() recibe un string de consola una ip*/
		public static int[] token(String consola){
			int i;
			i=0;
			int [] consolaToInt = new int[4];

			StringTokenizer tokens = new StringTokenizer(consola,".");
		
			while(tokens.hasMoreTokens() && i!=consolaToInt.length){
				consolaToInt [i]= Integer.parseInt(tokens.nextToken());
				
			System.out.println(consolaToInt[i]);i++;
			}
		return consolaToInt;
		}


		public static int and(int a,int b){
			return a&b;

		}
		public static int xor(int a,int b){
			return a&b;

		}
		public static int not(int a,int b){
			return a&b;

		}
		 public static int unsignedByteToInt(byte b) {
   			return (int) b & 0xFF;
  		}
	}
